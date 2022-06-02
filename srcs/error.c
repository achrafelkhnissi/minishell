/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fathjami <fathjami@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/29 16:53:22 by fathjami          #+#    #+#             */
/*   Updated: 2022/05/29 16:59:41 by fathjami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pipe_error(t_elem *ptr)
{
	t_elem	*prev;
	t_elem	*next;

	prev = skip_spaces(ptr->prev, 0);
	next = skip_spaces(ptr->next, 1);
	if ((!prev || !next) || (prev->type != WORD
			&& next->type != WORD && !is_redir(next->type)))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	redir_error(t_elem *ptr)
{
	t_elem	*next;

	next = skip_spaces(ptr->next, 1);
	if (!next || (next->type != WORD && next->type != ENV))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	ft_perror(char *msg, char *utils)
{
	write(STDERR_FILENO, msg, ft_strlen(msg));
	if (utils)
		write(STDERR_FILENO, utils, ft_strlen(utils));
	write(STDERR_FILENO, "\n", 1);
	return (EXIT_FAILURE);
}

int	ft_puterr(char *cmd, char *arg, char *msg, int err_num)
{
	write(STDERR_FILENO, "minishell: ", ft_strlen("minishell: "));
	write(STDERR_FILENO, cmd, ft_strlen(cmd));
	write(STDERR_FILENO, ": ", 2);
	write(STDERR_FILENO, arg, ft_strlen(arg));
	write(STDERR_FILENO, ": ", 2);
	if (!msg)
		perror("");
	else
		printf("%s\n", msg);
	g_data.exit_status = err_num;
	return (err_num);
}
