/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                         :+:      :+:    :+:   */
/*                                                  +:+ +:+         +:+      */
/*   By: elsahin <elsahin@student.42.fr>          +#+  +:+       +#+         */
/*                                              +#+#+#+#+#+   +#+            */
/*   Created: 2026/05/14 12:00:00 by elsahin           #+#    #+#            */
/*   Updated: 2026/05/17 12:00:00 by elsahin          ###   ########.fr      */
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

void	sim_abort_threads(t_sim *sim, int n_created)
{
	int	i;

	sim_set_stop(sim);
	dongles_wake_all(sim);
	i = 0;
	while (i < n_created)
	{
		pthread_join(sim->coders[i].thread, NULL);
		i++;
	}
}

void	sim_join_all(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->cfg.n_coders)
	{
		pthread_join(sim->coders[i].thread, NULL);
		i++;
	}
	pthread_join(sim->monitor, NULL);
}

void	sim_destroy(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->n_inited)
	{
		pthread_mutex_destroy(&sim->coders[i].state_lock);
		pthread_mutex_destroy(&sim->dongles[i].lock);
		pthread_cond_destroy(&sim->dongles[i].cond);
		pq_destroy(&sim->dongles[i].waiters);
		i++;
	}
	pthread_mutex_destroy(&sim->stop_lock);
	pthread_mutex_destroy(&sim->print_lock);
	free(sim->coders);
	free(sim->dongles);
	sim->coders = NULL;
	sim->dongles = NULL;
	sim->n_inited = 0;
}
