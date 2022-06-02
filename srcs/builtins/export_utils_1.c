/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils_1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fathjami <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 13:04:18 by fathjami          #+#    #+#             */
/*   Updated: 2022/05/30 13:04:20 by fathjami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}

void	ft_swap(t_env_elem *a, t_env_elem *b)
{
	char	*key_tmp;
	char	*val_tmp;

	key_tmp = a->key;
	val_tmp = a->value;
	a->key = b->key;
	a->value = b->value;
	b->key = key_tmp;
	b->value = val_tmp;
}

void	ft_sort(t_env *env)
{
	t_env_elem	*current;

	current = env->head;
	while (current->next)
	{
		if (ft_strcmp(current->key, current->next->key) > 0)
		{
			ft_swap(current, current->next);
			current = env->head;
		}
		else
			current = current->next;
	}
}

void	print_sorted_env(t_env *env)
{
	t_env		*tmp;
	t_env_elem	*current;

	tmp = env;
	ft_sort(tmp);
	current = tmp->head;
	while (current)
	{
		printf("declare -x %s", current->key);
		if (current->value)
			printf("=\"%s\"", current->value);
		printf("\n");
		current = current->next;
	}
}
