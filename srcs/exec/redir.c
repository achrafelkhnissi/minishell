/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fathjami <fathjami@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/25 08:30:58 by ael-khni          #+#    #+#             */
/*   Updated: 2022/05/30 11:56:35 by fathjami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_redir_in(t_redir_elem *tmp, int *in)
{
	if (access(tmp->arg, F_OK) == 0)
	{
		close(*in);
		*in = open(tmp->arg, O_RDONLY, 0666);
	}
	else
	{
		*in = -1;
		write(2, "minishell: ", 11);
		ft_perror(tmp->arg, ": No such file or directory");
	}
}

void	exec_redir(t_redir_list *list, t_fd *fd)
{
	t_redir_elem	*tmp;

	tmp = list->head;
	while (tmp)
	{
		if (tmp->type == REDIR_IN)
			exec_redir_in(tmp, &fd->in);
		else if (tmp->type == REDIR_OUT)
		{
			close(fd->out);
			(fd)->out = open(tmp->arg, O_WRONLY | O_CREAT | O_TRUNC, 0666);
		}
		else if (tmp->type == DREDIR_OUT)
		{
			close(fd->out);
			(fd)->out = open(tmp->arg, O_WRONLY | O_CREAT | O_APPEND, 0666);
		}
		else if (tmp->type == HERE_DOC)
		{
			close(fd->in);
			fd->in = open(tmp->arg, O_RDONLY, 0666);
		}
		tmp = tmp->next;
	}
}
