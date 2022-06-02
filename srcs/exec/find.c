/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fathjami <fathjami@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/29 17:34:44 by fathjami          #+#    #+#             */
/*   Updated: 2022/05/30 12:20:22 by fathjami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_slash_dot(char *cmd, char **env)
{
	char	*cmd_path;

	if (cmd[0] == '/')
	{
		if (opendir(cmd) != NULL)
			printf("minishell: %s: is a directory\n", cmd);
		else if (access(cmd, F_OK) == 0)
			return (cmd);
		else
			printf("minishell: %s: No such file or directory\n", cmd);
	}
	else if (cmd[0] == '.')
	{
		cmd_path = ft_join_dir(search_env_var("PWD", env), cmd);
		if (access(cmd_path, F_OK) == 0)
			return (cmd);
		else
			printf("minishell: %s: No such file or directory\n", cmd);
	}
	return (NULL);
}

char	*find_path(char *cmd, char **env)
{
	char	**paths;
	char	*path;
	char	*cmd_path;
	int		i;

	i = 0;
	path = search_env_var("PATH", env);
	paths = ft_split(path, ":");
	cmd_path = ft_strdup("");
	if (!cmd_path)
		return (NULL);
	while (paths[i])
	{
		cmd_path = ft_join_dir(paths[i], cmd);
		if (cmd_path)
		{
			if (access(cmd_path, F_OK) == 0)
				return (cmd_path);
			i++;
			free(cmd_path);
		}
	}
	printf("minishell: %s: command not found\n", cmd);
	return (NULL);
}

char	*find_command(char *cmd, char **env)
{
	char	*cmd_path;
	int		i;

	i = 0;
	if (cmd[0] == '/' || cmd[0] == '.')
		cmd_path = find_slash_dot(cmd, env);
	else
		cmd_path = find_path(cmd, env);
	if (cmd_path)
		return (cmd_path);
	exit(EXIT_FAILURE);
	return (NULL);
}

int	*find_right_left(t_ast_node *ptr)
{
	return (&ptr->content->pipe->right->content-> \
		pipe->left->content->cmd->fd.out);
}
