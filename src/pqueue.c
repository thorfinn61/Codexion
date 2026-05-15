/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pqueue.c                                          :+:      :+:    :+:   */
/*                                                  +:+ +:+         +:+      */
/*   By: elsahin <elsahin@student.42.fr>        +#+  +:+       +#+         */
/*                                              +#+#+#+#+#+   +#+            */
/*   Created: 2026/05/14 12:00:00 by elsahin          #+#    #+#            */
/*   Updated: 2026/05/15 12:00:00 by elsahin         ###   ########.fr      */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	pq_init(t_pqueue *pq, int capacity, int scheduler)
{
	pq->items = malloc(sizeof(t_request) * capacity);
	if (!pq->items)
		return (-1);
	pq->size = 0;
	pq->capacity = capacity;
	pq->scheduler = scheduler;
	return (0);
}

void	pq_destroy(t_pqueue *pq)
{
	free(pq->items);
	pq->items = NULL;
	pq->size = 0;
	pq->capacity = 0;
}

int	pq_push(t_pqueue *pq, t_request req)
{
	if (pq->size >= pq->capacity)
		return (-1);
	pq->items[pq->size] = req;
	pq->size++;
	heapify_up(pq, pq->size - 1);
	return (0);
}

int	pq_pop(t_pqueue *pq, t_request *out)
{
	if (pq->size == 0)
		return (-1);
	*out = pq->items[0];
	pq->size--;
	if (pq->size > 0)
	{
		pq->items[0] = pq->items[pq->size];
		heapify_down(pq, 0);
	}
	return (0);
}

int	pq_peek(t_pqueue *pq, t_request *out)
{
	if (pq->size == 0)
		return (-1);
	*out = pq->items[0];
	return (0);
}
