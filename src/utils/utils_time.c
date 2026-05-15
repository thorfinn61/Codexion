/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_time.c                                      :+:      :+:    :+:   */
/*                                                  +:+ +:+         +:+      */
/*   By: elsahin <elsahin@student.42.fr>          +#+  +:+       +#+         */
/*                                              +#+#+#+#+#+   +#+            */
/*   Created: 2026/05/14 12:00:00 by elsahin           #+#    #+#            */
/*   Updated: 2026/05/15 12:00:00 by elsahin          ###   ########.fr      */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((long)tv.tv_sec * 1000L + (long)tv.tv_usec / 1000L);
}

void	precise_sleep_ms(long ms)
{
	long	start;

	start = get_time_ms();
	while (get_time_ms() - start < ms)
		usleep(500);
}
