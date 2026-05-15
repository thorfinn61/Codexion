/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                           :+:      :+:    :+:   */
/*                                                  +:+ +:+         +:+      */
/*   By: elsahin <elsahin@student.42.fr>        +#+  +:+       +#+         */
/*                                              +#+#+#+#+#+   +#+            */
/*   Created: 2026/05/14 12:00:00 by elsahin          #+#    #+#            */
/*   Updated: 2026/05/15 12:00:00 by elsahin         ###   ########.fr      */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	safe_atol(const char *s, long *out)
{
	long	result;
	int		i;

	if (!s || !*s)
		return (-1);
	result = 0;
	i = 0;
	if (s[i] == '+')
		i++;
	if (!s[i])
		return (-1);
	while (s[i])
	{
		if (s[i] < '0' || s[i] > '9')
			return (-1);
		if (result > (2147483647L - (s[i] - '0')) / 10)
			return (-1);
		result = result * 10 + (s[i] - '0');
		i++;
	}
	*out = result;
	return (0);
}
