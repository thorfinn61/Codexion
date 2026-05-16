/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                           :+:      :+:    :+:   */
/*                                                  +:+ +:+         +:+      */
/*   By: elsahin <elsahin@student.42.fr>          +#+  +:+       +#+         */
/*                                              +#+#+#+#+#+   +#+            */
/*   Created: 2026/05/16 12:00:00 by elsahin           #+#    #+#            */
/*   Updated: 2026/05/16 12:00:00 by elsahin          ###   ########.fr      */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/*
** Brise Circular Wait (Coffman) : on acquiert toujours le dongle au plus petit
** id en premier. Deux coders qui veulent les memes deux dongles vont tous les
** deux essayer de prendre le plus petit d'abord — un seul reussit, pas de
** cycle. Si left == right (1 seul coder), le second acquire bloque et le
** monitor declenche le burnout : deterministe, pas un deadlock.
*/

int	coder_acquire_both(t_sim *sim, t_coder *c)
{
	int	first;
	int	second;

	if (c->left_dongle < c->right_dongle)
	{
		first = c->left_dongle;
		second = c->right_dongle;
	}
	else
	{
		first = c->right_dongle;
		second = c->left_dongle;
	}
	if (dongle_acquire(sim, c, first) != 0)
		return (-1);
	if (dongle_acquire(sim, c, second) != 0)
	{
		dongle_release(sim, c, first);
		return (-1);
	}
	return (0);
}

void	coder_release_both(t_sim *sim, t_coder *c)
{
	dongle_release(sim, c, c->left_dongle);
	if (c->left_dongle != c->right_dongle)
		dongle_release(sim, c, c->right_dongle);
}
