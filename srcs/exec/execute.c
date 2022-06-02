/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fathjami <fathjami@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/25 08:00:12 by ael-khni          #+#    #+#             */
/*   Updated: 2022/05/30 12:20:39 by fathjami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_builtins(char *cmd)
{
	if (cmd && (ft_strcmp(cmd, "export") == EQUAL))
		return (1);
	else if (cmd && (ft_strcmp(cmd, "unset") == EQUAL))
		return (1);
	else if (cmd && (ft_strcmp(cmd, "pwd") == EQUAL))
		return (1);
	else if (cmd && (ft_strcmp(cmd, "exit") == EQUAL))
		return (1);
	else if (cmd && (ft_strcmp(cmd, "cd") == EQUAL))
		return (1);
	else if (cmd && (ft_strcmp(cmd, "env") == EQUAL))
		return (1);
	else if (cmd && (ft_strcmp(cmd, "echo") == EQUAL))
		return (1);
	return (0);
}

void	ft_which_cmd(t_cmd *cmd, char ***env, t_elem *head)
{
	if (cmd->args[0] && (ft_strcmp(cmd->args[0], "export") == EQUAL))
		ft_export(env, cmd, head);
	else if (cmd->args[0] && (ft_strcmp(cmd->args[0], "unset") == EQUAL))
		ft_unset(cmd, env);
	else if (cmd->args[0] && (ft_strcmp(cmd->args[0], "pwd") == EQUAL))
		ft_pwd(cmd);
	else if (cmd->args[0] && (ft_strcmp(cmd->args[0], "exit") == EQUAL))
		ft_exit(cmd, *env);
	else if (cmd->args[0] && (ft_strcmp(cmd->args[0], "cd") == EQUAL))
		ft_cd(cmd, env);
	else if (cmd->args[0] && (ft_strcmp(cmd->args[0], "env") == EQUAL))
		ft_env(cmd, *env);
	else if (cmd->args[0] && (ft_strcmp(cmd->args[0], "echo") == EQUAL))
		ft_echo(cmd, head, *env);
}

void	check_last_pid(int pid, int count, int cmd_count)
{
	int	status;

	status = 0;
	if (count == cmd_count)
	{
		waitpid(pid, &status, 0);
		if (!g_data.which)
			g_data.exit_status = WEXITSTATUS(status);
	}
}

int	exec_builtins_execve(t_ast_node *ptr, char ***env, t_elem *head, int fd[2])
{
	int	pid;

	pid = 0;
	if (check_builtins(ptr->content->cmd->args[0]) == 1)
		execute_builtins(ptr->content->cmd, env, head);
	else
		pid = execute_cmd(ptr->content->cmd, *env, fd);
	return (pid);
}

void	execute(t_ast_node *ptr, char ***env, int cmd_count, t_elem *head)
{
	int			fd[2];
	int			pid;
	static int	count;

	pid = 0;
	if (ptr->type == CMD)
	{
		pid = exec_builtins_execve(ptr, env, head, fd);
		count++;
	}
	else if (ptr->type == PIPE)
	{
		if (pipe(fd) < 0)
			return ;
		if (ptr->content->pipe->right->type == CMD)
			dup_fd(&ptr->content->pipe->right->content->cmd->fd.out, fd[1]);
		else if (ptr->content->pipe->right->type == PIPE)
			dup_fd(find_right_left(ptr), fd[1]);
		dup_fd(&ptr->content->pipe->left->content->cmd->fd.in, fd[0]);
		execute(ptr->content->pipe->right, env, cmd_count, head);
		execute(ptr->content->pipe->left, env, cmd_count, head);
		close(fd[0]);
		close(fd[1]);
	}
	check_last_pid(pid, count, cmd_count);
}
