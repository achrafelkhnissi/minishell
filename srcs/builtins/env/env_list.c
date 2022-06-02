/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-khni <ael-khni@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/08 14:44:24 by fathjami          #+#    #+#             */
/*   Updated: 2022/05/25 08:26:09 by ael-khni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*init_env(t_env *env)
{
	env = ft_calloc(sizeof(t_env), 1);
	if (!env)
		return (NULL);
	return (env);
}

t_env_elem	*new_env_elem(char *line)
{
	t_env_elem	*elem;
	char		*key;
	char		*value;
	int			index;

	index = ft_strchr(line, '=');
	elem = ft_calloc(sizeof(t_env_elem), 1);
	if (index != -1)
	{
		key = ft_substr(line, 0, index);
		value = ft_substr(line, index + 1, ft_strlen(line) - index);
		if (!key || !value || !elem)
			return (NULL);
		elem->key = key;
		elem->value = value;
	}
	else
	{
		elem->key = ft_strdup(line);
		elem->value = NULL;
	}
	return (elem);
}

void	add_env_elem(t_env *env, t_env_elem *new)
{
	t_env_elem	*tmp;

	tmp = env->head;
	if (!env->head)
		env->head = new;
	else
	{
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
		new->prev = tmp;
	}
	env->size++;
}

// TODO: PROTECT INIT_ENV
t_env	*create_env(char **tab)
{
	t_env	*env;
	int		i;	

	env = NULL;
	env = init_env(env);
	env->env = tab;
	i = 0;
	while (tab[i])
		add_env_elem(env, new_env_elem(tab[i++]));
	return (env);
}
