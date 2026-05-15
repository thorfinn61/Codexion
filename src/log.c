/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.c                                             :+:      :+:    :+:   */
/*                                                  +:+ +:+         +:+      */
/*   By: elsahin <elsahin@student.42.fr>          +#+  +:+       +#+         */
/*                                              +#+#+#+#+#+   +#+            */
/*   Created: 2026/05/14 12:00:00 by elsahin           #+#    #+#            */
/*   Updated: 2026/05/15 12:00:00 by elsahin          ###   ########.fr      */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	log_state(t_sim *sim, int coder_id, const char *state)
{
	long	ts;

	pthread_mutex_lock(&sim->stop_lock);
	if (sim->stop_flag && strcmp(state, STATE_BURNED) != 0)
	{
		pthread_mutex_unlock(&sim->stop_lock);
		return ;
	}
	pthread_mutex_unlock(&sim->stop_lock);
	ts = get_time_ms() - sim->start_ms;
	pthread_mutex_lock(&sim->print_lock);
	printf("%ld %d %s\n", ts, coder_id, state);
	pthread_mutex_unlock(&sim->print_lock);
}
