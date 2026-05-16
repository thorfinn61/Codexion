/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                            :+:      :+:    :+:   */
/*                                                  +:+ +:+         +:+      */
/*   By: elsahin <elsahin@student.42.fr>          +#+  +:+       +#+         */
/*                                              +#+#+#+#+#+   +#+            */
/*   Created: 2026/05/14 12:00:00 by elsahin           #+#    #+#            */
/*   Updated: 2026/05/16 12:00:00 by elsahin          ###   ########.fr      */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	sim_should_stop(t_sim *sim)
{
	int	s;

	pthread_mutex_lock(&sim->stop_lock);
	s = sim->stop_flag;
	pthread_mutex_unlock(&sim->stop_lock);
	return (s);
}

void	sim_set_stop(t_sim *sim)
{
	pthread_mutex_lock(&sim->stop_lock);
	sim->stop_flag = 1;
	pthread_mutex_unlock(&sim->stop_lock);
}

static void	init_dongle(t_dongle *d, int id, t_config cfg)
{
	d->id = id;
	d->holder_id = -1;
	d->available_at_ms = 0;
	pthread_mutex_init(&d->lock, NULL);
	pthread_cond_init(&d->cond, NULL);
	pq_init(&d->waiters, cfg.n_coders + 1, cfg.scheduler);
}

int	sim_init(t_sim *sim)
{
	int	i;

	pthread_mutex_init(&sim->stop_lock, NULL);
	pthread_mutex_init(&sim->print_lock, NULL);
	sim->coders = malloc(sizeof(t_coder) * sim->cfg.n_coders);
	if (!sim->coders)
		return (-1);
	sim->dongles = malloc(sizeof(t_dongle) * sim->cfg.n_coders);
	if (!sim->dongles)
		return (-1);
	i = 0;
	while (i < sim->cfg.n_coders)
	{
		init_dongle(&sim->dongles[i], i, sim->cfg);
		sim->coders[i].id = i;
		sim->coders[i].left_dongle = i;
		sim->coders[i].right_dongle = (i + 1) % sim->cfg.n_coders;
		sim->coders[i].last_compile_start = 0;
		sim->coders[i].compiles_done = 0;
		sim->coders[i].sim = sim;
		pthread_mutex_init(&sim->coders[i].state_lock, NULL);
		i++;
	}
	return (0);
}

int	sim_start(t_sim *sim)
{
	int	i;

	sim->start_ms = get_time_ms();
	i = 0;
	while (i < sim->cfg.n_coders)
	{
		if (pthread_create(&sim->coders[i].thread, NULL,
				coder_routine, &sim->coders[i]) != 0)
			return (-1);
		i++;
	}
	if (pthread_create(&sim->monitor, NULL, monitor_routine, sim) != 0)
		return (-1);
	i = 0;
	while (i < sim->cfg.n_coders)
	{
		pthread_join(sim->coders[i].thread, NULL);
		i++;
	}
	pthread_join(sim->monitor, NULL);
	return (0);
}
