/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils_0.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fathjami <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 13:04:12 by fathjami          #+#    #+#             */
/*   Updated: 2022/05/30 13:04:15 by fathjami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**split_env(char *arg)
{
	char	**tmp;
	int		index;

	tmp = malloc(sizeof(char *) * 3);
	index = ft_strchr(arg, '=');
	tmp[0] = ft_substr(arg, 0, index);
	tmp[1] = ft_substr(arg, index + 1, ft_strlen(arg) - index);
	tmp[2] = 0;
	if (!tmp || !tmp[0] || !tmp[1])
		return (NULL);
	return (tmp);
}

int	check_equal_sign(char **args)
{
	int	i;

	i = 0;
	while (args[i])
	{
		if (ft_strchr(args[i], '=') == ft_strlen(args[i]) - 1)
			return (1);
		i++;
	}
	return (0);
}

t_elem	*find_env_key(t_elem *head, char *arg, char **env)
{
	t_elem	*tmp;

	tmp = head;
	while (tmp)
	{
		if (tmp->type == ENV)
		{
			if (ft_strcmp(expand_env(tmp->content, env), arg) == EQUAL)
				break ;
		}
		if (ft_strcmp(tmp->content, arg) == EQUAL)
			break ;
		tmp = tmp->next;
	}
	return (tmp);
}

void	handle_state(t_elem **ptr, char **env, int *value)
{
	if ((*ptr)->state == GENERAL)
	{
		(*value)++;
		if ((*ptr)->type == ENV)
		{
			if (ft_strcmp(expand_env((*ptr)->content, env), "") == EQUAL)
				(*value)--;
		}
	}
	else
	{
		while ((*ptr) && (*ptr)->state != GENERAL)
			(*ptr) = (*ptr)->next;
		(*value)++;
	}
}

int	check_export_cmd(t_elem *ptr, char **env)
{
	int	value;

	value = 0;
	while (ptr && ptr->type != PIPE_LINE && !is_redir(ptr->type)
		&& ptr->type != WHITE_SPACE)
	{
		if (ptr->type == WORD || ptr->type == ENV)
			handle_state(&ptr, env, &value);
		ptr = ptr->next;
	}
	return (value);
}
