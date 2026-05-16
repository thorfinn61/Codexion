/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                        :+:      :+:    :+:   */
/*                                                  +:+ +:+         +:+      */
/*   By: elsahin <elsahin@student.42.fr>        +#+  +:+       +#+         */
/*                                              +#+#+#+#+#+   +#+            */
/*   Created: 2026/05/14 12:00:00 by elsahin          #+#    #+#            */
/*   Updated: 2026/05/15 12:00:00 by elsahin         ###   ########.fr      */
/*                                                                            */
/* ************************************************************************** */

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

struct	s_sim;

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

int		safe_atol(const char *s, long *out);
int		parse_args(int ac, char **av, t_config *cfg);

long	get_time_ms(void);
void	precise_sleep_ms(long ms);
void	log_state(t_sim *sim, int coder_id, const char *state);

int		has_priority(t_request *a, t_request *b, int scheduler);
void	swap_req(t_request *a, t_request *b);
void	heapify_up(t_pqueue *pq, int i);
void	heapify_down(t_pqueue *pq, int i);

int		pq_init(t_pqueue *pq, int capacity, int scheduler);
void	pq_destroy(t_pqueue *pq);
int		pq_push(t_pqueue *pq, t_request req);
int		pq_pop(t_pqueue *pq, t_request *out);
int		pq_peek(t_pqueue *pq, t_request *out);

int		sim_should_stop(t_sim *sim);
void	sim_set_stop(t_sim *sim);
int		sim_init(t_sim *sim);
int		sim_start(t_sim *sim);
void	sim_destroy(t_sim *sim);

int		dongle_acquire(t_sim *sim, t_coder *c, int dongle_id);
void	dongle_release(t_sim *sim, t_coder *c, int dongle_id);

int		coder_acquire_both(t_sim *sim, t_coder *c);
void	coder_release_both(t_sim *sim, t_coder *c);
void	*coder_routine(void *arg);
void	*monitor_routine(void *arg);

#endif
