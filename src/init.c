/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                            :+:      :+:    :+:   */
/*                                                  +:+ +:+         +:+      */
/*   By: elsahin <elsahin@student.42.fr>        +#+  +:+       +#+         */
/*                                              +#+#+#+#+#+   +#+            */
/*   Created: 2026/05/14 12:00:00 by elsahin          #+#    #+#            */
/*   Updated: 2026/05/15 12:00:00 by elsahin         ###   ########.fr      */
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

int	sim_init(t_sim *sim)
{
	(void)sim;
	return (0);
}

int	sim_start(t_sim *sim)
{
	(void)sim;
	return (0);
}
