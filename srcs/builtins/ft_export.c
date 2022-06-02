/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fathjami <fathjami@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/25 08:03:01 by ael-khni          #+#    #+#             */
/*   Updated: 2022/05/30 13:00:05 by fathjami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	join_args(t_cmd **cmd, t_elem *ptr, char **env, int i)
{
	int	k;
	int	value;

	k = 0;
	value = check_export_cmd(ptr, env);
	while (++k < value && (*cmd)->args[i + k])
		(*cmd)->args[i] = ft_strjoin((*cmd)->args[i], (*cmd)->args[i + k]);
	free_2d(env);
	return (value);
}

void	check_index(t_env *env, char *args_i, char **tmp, int index)
{
	t_env_elem	*env_elem;

	env_elem = NULL;
	if (index != -1)
	{
		tmp = split_env(args_i);
		env_elem = search_env_elem(env, tmp[0]);
		if (env_elem)
		{
			free(env_elem->value);
			env_elem->value = ft_strdup(tmp[1]);
		}
		else
			add_env_elem(env, new_env_elem(args_i));
		free_2d(tmp);
	}
	else
	{
		env_elem = search_env_elem(env, args_i);
		if (!env_elem)
			add_env_elem(env, new_env_elem(args_i));
	}
}

void	handle_env(t_env *env, t_cmd *cmd, t_elem *head)
{
	char		**tmp;
	int			i;
	int			plus;
	int			index;

	i = 1;
	tmp = NULL;
	while (cmd->args[i])
	{
		plus = 0;
		plus += join_args(&cmd, find_env_key(head, cmd->args[i],
					convert_array(env)), convert_array(env), i);
		index = ft_strchr(cmd->args[i], '=');
		check_index(env, cmd->args[i], tmp, index);
		if (!plus)
			i++;
		else
			i += plus;
	}
}

int	ft_isalpha(char c)
{
	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

int	ft_export(char ***ev, t_cmd *cmd, t_elem *head)
{
	t_env	*env;
	int		i;

	i = 0;
	env = NULL;
	env = create_env(*ev);
	if (!cmd->args[1])
		print_sorted_env(env);
	else
	{
		if (!ft_isalpha(cmd->args[1][0]))
			return (ft_puterr(cmd->args[0],
					cmd->args[1], "not a valid identifier", EXIT_FAILURE));
		handle_env(env, cmd, head);
		g_data.env = convert_array(env);
		*ev = convert_array(env);
		g_data.alloc = 1;
	}
	del_env(env);
	g_data.exit_status = EXIT_SUCCESS;
	return (EXIT_SUCCESS);
}
