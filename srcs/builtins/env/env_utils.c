/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fathjami <fathjami@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/08 14:47:04 by fathjami          #+#    #+#             */
/*   Updated: 2022/05/29 17:40:10 by fathjami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	print_env(t_env *env)
{
	t_env_elem	*tmp;

	tmp = env->head;
	while (tmp)
	{
		printf("%s=%s\n", tmp->key, tmp->value);
		tmp = tmp->next;
	}
}

void	del_env_elem(t_env *env, t_env_elem *elem)
{
	if (elem && elem->next)
		elem->next->prev = elem->prev;
	if (elem && elem->prev)
		elem->prev->next = elem->next;
	env->size--;
	free(elem->key);
	free(elem->value);
	free(elem);
	elem = NULL;
}

void	del_env(t_env *env)
{
	t_env_elem	*tmp;

	tmp = env->head;
	while (tmp)
	{
		free(tmp->key);
		free(tmp->value);
		free(tmp);
		tmp = tmp->next;
	}
	free(env);
}

void	print_env_arr(char **env)
{
	int	i;

	i = 0;
	while (env[i])
		printf("%s\n", env[i++]);
}

char	**convert_array(t_env *env)
{
	t_env_elem	*tmp;
	char		**arr;
	int			i;

	i = 0;
	tmp = env->head;
	arr = ft_calloc(env->size + 1, sizeof(char *));
	if (arr)
	{
		while (i < env->size)
		{
			arr[i] = ft_strdup(tmp->key);
			if (tmp->value)
			{
				arr[i] = ft_strjoin(arr[i], "=");
				arr[i] = ft_strjoin(arr[i], tmp->value);
			}
			tmp = tmp->next;
			i++;
		}
		arr[i] = NULL;
	}
	return (arr);
}
