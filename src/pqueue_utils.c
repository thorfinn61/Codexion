/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pqueue_utils.c                                    :+:      :+:    :+:   */
/*                                                  +:+ +:+         +:+      */
/*   By: elsahin <elsahin@student.42.fr>        +#+  +:+       +#+         */
/*                                              +#+#+#+#+#+   +#+            */
/*   Created: 2026/05/14 12:00:00 by elsahin          #+#    #+#            */
/*   Updated: 2026/05/15 12:00:00 by elsahin         ###   ########.fr      */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	has_priority(t_request *a, t_request *b, int scheduler)
{
	if (scheduler == SCHED_EDF_POLICY)
	{
		if (a->deadline != b->deadline)
			return (a->deadline < b->deadline);
		return (a->coder_id < b->coder_id);
	}
	if (a->request_time != b->request_time)
		return (a->request_time < b->request_time);
	return (a->coder_id < b->coder_id);
}

void	swap_req(t_request *a, t_request *b)
{
	t_request	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

void	heapify_up(t_pqueue *pq, int i)
{
	int	parent;

	while (i > 0)
	{
		parent = (i - 1) / 2;
		if (has_priority(&pq->items[i], &pq->items[parent], pq->scheduler))
		{
			swap_req(&pq->items[i], &pq->items[parent]);
			i = parent;
		}
		else
			break ;
	}
}

void	heapify_down(t_pqueue *pq, int i)
{
	int	left;
	int	right;
	int	best;

	while (1)
	{
		left = 2 * i + 1;
		right = 2 * i + 2;
		best = i;
		if (left < pq->size
			&& has_priority(&pq->items[left], &pq->items[best], pq->scheduler))
			best = left;
		if (right < pq->size
			&& has_priority(&pq->items[right], &pq->items[best], pq->scheduler))
			best = right;
		if (best == i)
			break ;
		swap_req(&pq->items[i], &pq->items[best]);
		i = best;
	}
}
