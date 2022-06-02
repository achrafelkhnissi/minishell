/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fathjami <fathjami@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/25 08:00:39 by ael-khni          #+#    #+#             */
/*   Updated: 2022/05/29 18:02:49 by fathjami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_opt(char *opt)
{
	int	i;

	i = 1;
	if (opt[0] != '-')
		return (0);
	while (opt[i])
	{
		if (opt[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

int	check_echo_args(char **args)
{
	int	i;

	i = 1;
	while (args[i])
	{
		if (!is_opt(args[i]))
			return (i);
		i++;
		if (args[i] == NULL)
			return (i);
	}
	return (-1);
}

int	check__cmd(t_elem *ptr, char **env)
{
	int	value;

	value = 0;
	while (ptr && ptr->type != PIPE_LINE
		&& !is_redir(ptr->type) && ptr->type != WHITE_SPACE)
	{
		if (ptr->type == WORD || ptr->type == ENV)
			check_cmd_state(&ptr, env, &value);
		ptr = ptr->next;
	}
	return (value);
}

int	join_for_echo(t_cmd **cmd, t_elem *ptr, char **env, int i)
{
	int	k;
	int	value;

	k = 0;
	value = check__cmd(ptr, env);
	while (++k < value && (*cmd)->args[i + k])
		(*cmd)->args[i] = ft_strjoin((*cmd)->args[i], (*cmd)->args[i + k]);
	return (value);
}

int	ft_echo(t_cmd *cmd, t_elem *lex, char **env)
{
	int		i;
	int		plus;
	t_bool	opt;

	opt = false;
	i = check_echo_args(cmd->args);
	if (i <= 1)
		i = 1;
	else
		opt = true;
	while (cmd->args[i])
	{
		plus = 0;
		plus = join_for_echo(&cmd,
				find_env_key(lex, cmd->args[i], env), env, i);
		write(STDOUT_FILENO, cmd->args[i], ft_strlen(cmd->args[i]));
		ft_plus(&i, plus);
		if (cmd->args[i])
			write(STDOUT_FILENO, " ", 1);
	}
	if (!opt)
		write(STDOUT_FILENO, "\n", 1);
	g_data.exit_status = EXIT_SUCCESS;
	return (EXIT_SUCCESS);
}
