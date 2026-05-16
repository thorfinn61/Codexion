/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                          :+:      :+:    :+:   */
/*                                                  +:+ +:+         +:+      */
/*   By: elsahin <elsahin@student.42.fr>          +#+  +:+       +#+         */
/*                                              +#+#+#+#+#+   +#+            */
/*   Created: 2026/05/14 12:00:00 by elsahin           #+#    #+#            */
/*   Updated: 2026/05/15 12:00:00 by elsahin          ###   ########.fr      */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	dongle_acquire(t_sim *sim, t_coder *c, int dongle_id)
{
	t_dongle	*d;
	t_request	req;

	d = &sim->dongles[dongle_id];
	build_request(sim, c, &req);
	pthread_mutex_lock(&d->lock);
	pq_push(&d->waiters, req);
	while (1)
	{
		if (sim_should_stop(sim))
		{
			pthread_mutex_unlock(&d->lock);
			return (-1);
		}
		if (can_take(d, c->id, get_time_ms()))
			break ;
		wait_for_dongle(d, get_time_ms());
	}
	d->holder_id = c->id;
	pthread_mutex_unlock(&d->lock);
	log_state(sim, c->id, STATE_TAKEN);
	return (0);
}

void	dongle_release(t_sim *sim, t_coder *c, int dongle_id)
{
	t_dongle	*d;

	(void)c;
	d = &sim->dongles[dongle_id];
	pthread_mutex_lock(&d->lock);
	d->holder_id = -1;
	d->available_at_ms = get_time_ms() + sim->cfg.dongle_cooldown;
	pthread_cond_broadcast(&d->cond);
	pthread_mutex_unlock(&d->lock);
}

void	dongles_wake_all(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->cfg.n_coders)
	{
		pthread_mutex_lock(&sim->dongles[i].lock);
		pthread_cond_broadcast(&sim->dongles[i].cond);
		pthread_mutex_unlock(&sim->dongles[i].lock);
		i++;
	}
}
