/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fathjami <fathjami@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/23 18:21:34 by ael-khni          #+#    #+#             */
/*   Updated: 2022/04/10 14:59:25 by fathjami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_empty(t_list *list)
{
	return (list->head == NULL);
}

t_list	*init_list(t_list *list)
{
	list = ft_calloc(sizeof(t_list), 1);
	if (!list)
		return (NULL);
	return (list);
}

t_elem	*new_elem(char *content, int len, enum e_token type, enum e_state state)
{
	t_elem	*elem;
	char	*cont;

	elem = ft_calloc(sizeof(t_elem), 1);
	if (!elem)
		return (NULL);
	cont = ft_strndup(content, len);
	if (!cont)
		return (NULL);
	elem->content = cont;
	elem->len = len;
	elem->type = type;
	elem->state = state;
	return (elem);
}

void	add_tail(t_list *list, t_elem *new)
{
	if (is_empty(list))
		list->head = new;
	else
	{
		list->tail->next = new;
		new->prev = list->tail;
	}
	list->tail = new;
	list->size++;
}

void	free_list(t_list *list)
{
	t_elem	*tmp;

	tmp = list->head;
	while (tmp)
	{
		free(tmp->content);
		free(tmp);
		tmp = tmp->next;
	}
	free(list);
}
