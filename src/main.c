#include "codexion.h"

int	main(int ac, char **av)
{
	t_config	cfg;

	if (parse_args(ac, av, &cfg) != 0)
	{
		fprintf(stderr, "Usage: %s n_coders t_burnout t_compile t_debug "
			"t_refactor n_compiles_required dongle_cooldown {fifo|edf}\n",
			av[0]);
		return (1);
	}
	printf("OK: %d coders, burnout=%ld, compile=%ld, debug=%ld, "
		"refactor=%ld, target=%d, cooldown=%ld, sched=%s\n",
		cfg.n_coders, cfg.t_burnout, cfg.t_compile, cfg.t_debug,
		cfg.t_refactor, cfg.n_compiles_required, cfg.dongle_cooldown,
		(cfg.scheduler == SCHED_FIFO_POLICY) ? "fifo" : "edf");
	return (0);
}
