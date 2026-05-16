/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.c                                             :+:      :+:    :+:   */
/*                                                  +:+ +:+         +:+      */
/*   By: elsahin <elsahin@student.42.fr>          +#+  +:+       +#+         */
/*                                              +#+#+#+#+#+   +#+            */
/*   Created: 2026/05/14 12:00:00 by elsahin           #+#    #+#            */
/*   Updated: 2026/05/16 12:00:00 by elsahin          ###   ########.fr      */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

#define COLOR_RESET		"\033[0m"
#define COLOR_TAKEN		"\033[36m"
#define COLOR_COMPILE	"\033[32m"
#define COLOR_DEBUG		"\033[33m"
#define COLOR_REFACTOR	"\033[35m"
#define COLOR_BURNED	"\033[31m"

static const char	*state_color(const char *state)
{
	if (!strcmp(state, STATE_BURNED))
		return (COLOR_BURNED);
	if (!strcmp(state, STATE_COMPILING))
		return (COLOR_COMPILE);
	if (!strcmp(state, STATE_DEBUGGING))
		return (COLOR_DEBUG);
	if (!strcmp(state, STATE_REFACTORING))
		return (COLOR_REFACTOR);
	return (COLOR_TAKEN);
}

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
	printf("%s%ld %d %s%s\n", state_color(state), ts, coder_id, state,
		COLOR_RESET);
	pthread_mutex_unlock(&sim->print_lock);
}
