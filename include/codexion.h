#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

# define SCHED_FIFO_POLICY	0
# define SCHED_EDF_POLICY	1

# define STATE_TAKEN		"has taken a dongle"
# define STATE_COMPILING	"is compiling"
# define STATE_DEBUGGING	"is debugging"
# define STATE_REFACTORING	"is refactoring"
# define STATE_BURNED		"burned out"

typedef struct s_config
{
	int		n_coders;
	long	t_burnout;
	long	t_compile;
	long	t_debug;
	long	t_refactor;
	int		n_compiles_required;
	long	dongle_cooldown;
	int		scheduler;
}	t_config;

typedef struct s_request
{
	int		coder_id;
	long	request_time;
	long	deadline;
}	t_request;

typedef struct s_pqueue
{
	t_request	*items;
	int			size;
	int			capacity;
	int			scheduler;
}	t_pqueue;

struct s_sim;

typedef struct s_dongle
{
	int				id;
	pthread_mutex_t	lock;
	pthread_cond_t	cond;
	long			available_at_ms;
	int				holder_id;
	t_pqueue		waiters;
}	t_dongle;

typedef struct s_coder
{
	int				id;
	int				left_dongle;
	int				right_dongle;
	long			last_compile_start;
	int				compiles_done;
	pthread_t		thread;
	pthread_mutex_t	state_lock;
	struct s_sim	*sim;
}	t_coder;

typedef struct s_sim
{
	t_config		cfg;
	t_coder			*coders;
	t_dongle		*dongles;
	long			start_ms;
	int				stop_flag;
	pthread_mutex_t	stop_lock;
	pthread_mutex_t	print_lock;
	pthread_t		monitor;
}	t_sim;

#endif
