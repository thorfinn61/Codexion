*This project has been created as part of the 42 curriculum by elsahin.*

---

## Description

Codexion is a concurrent simulation inspired by the Dining Philosophers problem. N coders sit around a table, each needing two USB dongles to compile. Dongles are shared with neighbors in a circular arrangement. Each coder loops through three phases — compile, debug, refactor — and burns out if it goes too long without compiling.

The program explores thread synchronization, deadlock prevention, and real-time scheduling using POSIX threads.

## Instructions

```bash
make
./codexion n_coders t_burnout t_compile t_debug t_refactor n_compiles_required dongle_cooldown {fifo|edf}
```

| Argument | Description |
|---|---|
| `n_coders` | Number of coders (≥ 1) |
| `t_burnout` | Time in ms before a coder burns out without compiling |
| `t_compile` | Time in ms to compile |
| `t_debug` | Time in ms to debug |
| `t_refactor` | Time in ms to refactor |
| `n_compiles_required` | Number of compiles each coder must complete (0 = unlimited) |
| `dongle_cooldown` | Time in ms a dongle is unavailable after release |
| `scheduler` | `fifo` (first come first served) or `edf` (earliest deadline first) |

**Examples**

```bash
# 1 coder — always burns out (can't acquire 2 dongles alone)
./codexion 1 800 200 200 200 5 50 fifo

# 2 coders — survives 3 compile cycles each
./codexion 2 800 200 100 100 3 0 fifo

# 5 coders — EDF scheduler prioritizes the most urgent coder
./codexion 5 1200 200 200 200 5 0 edf
```

**Output format**

Each line is: `timestamp_ms coder_id state`

```
0 0 has taken a dongle
0 0 has taken a dongle
0 0 is compiling
200 0 is debugging
...
```

## Resources

- [POSIX Threads Programming — Lawrence Livermore National Laboratory](https://hpc-tutorials.llnl.gov/posix/)
- [Dining Philosophers Problem — Wikipedia](https://en.wikipedia.org/wiki/Dining_philosophers_problem)
- [Earliest Deadline First Scheduling — Wikipedia](https://en.wikipedia.org/wiki/Earliest_deadline_first_scheduling)
- [Coffman Conditions for Deadlock — Wikipedia](https://en.wikipedia.org/wiki/Deadlock#Coffman_conditions)

*This project was developed with the assistance of Claude (Anthropic) for architecture design, norminette compliance checking, and debugging of synchronization issues.*

## Blocking cases handled

**Deadlock**
To compile, each coder must hold two dongles simultaneously. Without ordering, a circular wait can occur where every coder holds one dongle and waits for its neighbor's. This satisfies all four Coffman conditions (mutual exclusion, hold and wait, no preemption, circular wait). We break circular wait by always acquiring the dongle with the lower id first. Both coders competing for the same pair will attempt the lower id first — only one succeeds, so no cycle forms.

**Starvation**
With FIFO scheduling, a coder with a later request time will always wait behind earlier ones, even if it is close to burning out. EDF (Earliest Deadline First) prevents this by prioritizing the coder whose deadline (`last_compile_start + t_burnout`) is closest. Under feasible parameters, no coder is skipped indefinitely.

**Cooldown**
After a dongle is released, it becomes unavailable for `dongle_cooldown` ms. The acquire loop checks `available_at_ms` before granting the dongle. This is enforced through `can_take` inside the mutex-protected condition wait loop.

**Burnout precision**
The monitor thread polls every 200 µs and detects burnout within 10 ms of the exact threshold. When burnout is detected, it broadcasts on all dongle condition variables to immediately unblock any coder stuck in `pthread_cond_wait`.

**Log serialization**
All output goes through a single `print_lock` mutex. Without it, partial lines from concurrent threads would interleave. The `stop_lock` is checked separately before printing to avoid logging stale states after the simulation ends.

## Thread synchronization mechanisms

**`pthread_mutex_t`**
Used to protect every piece of shared state:
- `dongle.lock` — guards `holder_id`, `available_at_ms`, and the priority queue of waiters
- `coder.state_lock` — guards `last_compile_start` and `compiles_done`
- `sim.stop_lock` — guards `stop_flag` (read by all threads, written by the monitor)
- `sim.print_lock` — serializes all `printf` calls to prevent interleaved output

Without `dongle.lock`, two coders could both read `holder_id == -1` and both believe the dongle is free — a classic TOCTOU race. Without `state_lock`, the monitor could read a partially updated `last_compile_start` and produce a false burnout detection.

**`pthread_cond_t`**
Each dongle has a condition variable (`dongle.cond`). When a coder cannot acquire a dongle (holder busy, cooldown active, or not at queue head), it sleeps on `pthread_cond_wait` instead of spinning. When a dongle is released, `pthread_cond_broadcast` wakes all waiters. When the simulation stops, `dongles_wake_all` broadcasts on every dongle so no thread stays blocked indefinitely.
