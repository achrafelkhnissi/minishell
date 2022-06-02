/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-khni <ael-khni@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/25 08:23:36 by ael-khni          #+#    #+#             */
/*   Updated: 2022/05/25 08:46:55 by ael-khni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_pwd(t_cmd *cmd)
{
	char	cwd[1024];
	char	*dir;

	(void)cmd;
	dir = getcwd(cwd, sizeof(cwd));
	if (!dir)
	{
		perror("minishell: pwd: ");
		g_data.exit_status = EXIT_FAILURE;
		return (-1);
	}
	printf("%s\n", dir);
	g_data.exit_status = EXIT_SUCCESS;
	return (EXIT_SUCCESS);
}
