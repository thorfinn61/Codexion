/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                         :+:      :+:    :+:   */
/*                                                  +:+ +:+         +:+      */
/*   By: elsahin <elsahin@student.42.fr>          +#+  +:+       +#+         */
/*                                              +#+#+#+#+#+   +#+            */
/*   Created: 2026/05/16 12:00:00 by elsahin           #+#    #+#            */
/*   Updated: 2026/05/16 12:00:00 by elsahin          ###   ########.fr      */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	monitor_check_burnout(t_sim *sim, t_coder *c, long now)
{
	long	last;

	pthread_mutex_lock(&c->state_lock);
	last = c->last_compile_start;
	pthread_mutex_unlock(&c->state_lock);
	if (last == 0)
		last = sim->start_ms;
	if (now - last > sim->cfg.t_burnout)
	{
		log_state(sim, c->id, STATE_BURNED);
		sim_set_stop(sim);
		return (1);
	}
	return (0);
}

static int	monitor_check_done(t_sim *sim)
{
	int	i;
	int	done;

	if (sim->cfg.n_compiles_required <= 0)
		return (0);
	i = 0;
	while (i < sim->cfg.n_coders)
	{
		pthread_mutex_lock(&sim->coders[i].state_lock);
		done = sim->coders[i].compiles_done >= sim->cfg.n_compiles_required;
		pthread_mutex_unlock(&sim->coders[i].state_lock);
		if (!done)
			return (0);
		i++;
	}
	return (1);
}

static int	monitor_poll(t_sim *sim, long now)
{
	int	i;

	i = 0;
	while (i < sim->cfg.n_coders)
	{
		if (monitor_check_burnout(sim, &sim->coders[i], now))
		{
			dongles_wake_all(sim);
			return (1);
		}
		i++;
	}
	if (monitor_check_done(sim))
	{
		sim_set_stop(sim);
		dongles_wake_all(sim);
		return (1);
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_sim	*sim;

	sim = (t_sim *)arg;
	while (!sim_should_stop(sim))
	{
		usleep(200);
		if (monitor_poll(sim, get_time_ms()))
			return (NULL);
	}
	return (NULL);
}
