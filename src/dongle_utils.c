/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_utils.c                                     :+:      :+:    :+:   */
/*                                                  +:+ +:+         +:+      */
/*   By: elsahin <elsahin@student.42.fr>          +#+  +:+       +#+         */
/*                                              +#+#+#+#+#+   +#+            */
/*   Created: 2026/05/16 12:00:00 by elsahin           #+#    #+#            */
/*   Updated: 2026/05/16 12:00:00 by elsahin          ###   ########.fr      */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	build_request(t_sim *sim, t_coder *c, t_request *req)
{
	long	last;

	req->coder_id = c->id;
	req->request_time = get_time_ms();
	pthread_mutex_lock(&c->state_lock);
	last = c->last_compile_start;
	pthread_mutex_unlock(&c->state_lock);
	if (last == 0)
		last = sim->start_ms;
	req->deadline = last + sim->cfg.t_burnout;
}

int	can_take(t_dongle *d, int coder_id, long now)
{
	t_request	head;
	t_request	popped;

	if (d->holder_id != -1)
		return (0);
	if (now < d->available_at_ms)
		return (0);
	if (pq_peek(&d->waiters, &head) != 0)
		return (0);
	if (head.coder_id != coder_id)
		return (0);
	pq_pop(&d->waiters, &popped);
	return (1);
}

void	wait_for_dongle(t_dongle *d, long now)
{
	struct timespec	ts;
	long			wait_ms;

	if (d->holder_id == -1 && now < d->available_at_ms)
	{
		wait_ms = d->available_at_ms - now;
		clock_gettime(CLOCK_REALTIME, &ts);
		ts.tv_sec += (ts.tv_nsec / 1000000L + wait_ms) / 1000L;
		ts.tv_nsec = ((ts.tv_nsec / 1000000L + wait_ms) % 1000L) * 1000000L;
		pthread_cond_timedwait(&d->cond, &d->lock, &ts);
	}
	else
		pthread_cond_wait(&d->cond, &d->lock);
}
