/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                         :+:      :+:    :+:   */
/*                                                  +:+ +:+         +:+      */
/*   By: elsahin <elsahin@student.42.fr>          +#+  +:+       +#+         */
/*                                              +#+#+#+#+#+   +#+            */
/*   Created: 2026/05/14 12:00:00 by elsahin           #+#    #+#            */
/*   Updated: 2026/05/16 12:00:00 by elsahin          ###   ########.fr      */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	sim_destroy(t_sim *sim)
{
	int	i;

	i = 0;
	if (sim->coders && sim->dongles)
	{
		while (i < sim->cfg.n_coders)
		{
			pthread_mutex_destroy(&sim->coders[i].state_lock);
			pthread_mutex_destroy(&sim->dongles[i].lock);
			pthread_cond_destroy(&sim->dongles[i].cond);
			pq_destroy(&sim->dongles[i].waiters);
			i++;
		}
	}
	pthread_mutex_destroy(&sim->stop_lock);
	pthread_mutex_destroy(&sim->print_lock);
	free(sim->coders);
	free(sim->dongles);
	sim->coders = NULL;
	sim->dongles = NULL;
}
