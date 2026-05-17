/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                            :+:      :+:    :+:   */
/*                                                  +:+ +:+         +:+      */
/*   By: elsahin <elsahin@student.42.fr>          +#+  +:+       +#+         */
/*                                              +#+#+#+#+#+   +#+            */
/*   Created: 2026/05/14 12:00:00 by elsahin           #+#    #+#            */
/*   Updated: 2026/05/17 12:00:00 by elsahin          ###   ########.fr      */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	init_dongle(t_dongle *d, int id, t_config cfg)
{
	d->id = id;
	d->holder_id = -1;
	d->available_at_ms = 0;
	pthread_mutex_init(&d->lock, NULL);
	pthread_cond_init(&d->cond, NULL);
	if (pq_init(&d->waiters, cfg.n_coders + 1, cfg.scheduler) != 0)
	{
		pthread_mutex_destroy(&d->lock);
		pthread_cond_destroy(&d->cond);
		return (-1);
	}
	return (0);
}

static void	init_coder(t_coder *c, t_sim *sim, int i)
{
	c->id = i;
	c->left_dongle = i;
	c->right_dongle = (i + 1) % sim->cfg.n_coders;
	c->last_compile_start = 0;
	c->compiles_done = 0;
	c->sim = sim;
	pthread_mutex_init(&c->state_lock, NULL);
}

int	sim_init(t_sim *sim)
{
	int	i;

	pthread_mutex_init(&sim->stop_lock, NULL);
	pthread_mutex_init(&sim->print_lock, NULL);
	sim->coders = malloc(sizeof(t_coder) * sim->cfg.n_coders);
	sim->dongles = malloc(sizeof(t_dongle) * sim->cfg.n_coders);
	if (!sim->coders || !sim->dongles)
		return (-1);
	i = 0;
	while (i < sim->cfg.n_coders)
	{
		if (init_dongle(&sim->dongles[i], i, sim->cfg) != 0)
			return (-1);
		init_coder(&sim->coders[i], sim, i);
		sim->n_inited = i + 1;
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
		{
			sim_abort_threads(sim, i);
			return (-1);
		}
		i++;
	}
	if (pthread_create(&sim->monitor, NULL, monitor_routine, sim) != 0)
	{
		sim_abort_threads(sim, sim->cfg.n_coders);
		return (-1);
	}
	sim_join_all(sim);
	return (0);
}
