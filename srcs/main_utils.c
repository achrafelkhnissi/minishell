/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fathjami <fathjami@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/29 16:38:00 by fathjami          #+#    #+#             */
/*   Updated: 2022/05/29 17:13:40 by fathjami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	cmd_count(t_ast_node *ptr)
{
	static int	count;

	if (ptr->type == CMD)
		count++;
	else
	{
		cmd_count(ptr->content->pipe->right);
		cmd_count(ptr->content->pipe->left);
	}
	return (count);
}

void	ft_wait(void)
{
	int	status;

	status = 0;
	while (1)
	{
		g_data.pid = waitpid(-1, &status, 0);
		if (g_data.pid == -1 && errno == EINTR)
			continue ;
		if (g_data.pid == -1)
			break ;
	}
	if (WIFSIGNALED(status))
	{
		g_data.exit_status = WTERMSIG(status) + 128;
		if (WTERMSIG(status) == SIGINT)
			write(STDOUT_FILENO, "\n", 1);
	}
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGQUIT)
		printf("%s: %i\n", g_data.siglist[status], status);
	g_data.which = 0;
}

void	set_g_data(char **env)
{
		g_data.env = env;
		g_data.ev = create_env(env);
		g_data.tree = init_tree(g_data.tree);
		g_data.shell_state = SH_READING;
}

int	ft_readline(char **line)
{
	*line = readline("[minishell][:)]~> ");
		g_data.shell_state = SH_EXECUTING;
	if (!*line)
	{
		printf("exit\n");
		exit(1);
	}
	if (strcmp(*line, "") == EQUAL || ft_strisspace(*line))
		return (1);
	if (strlen(*line) > 0)
		add_history(*line);
	return (0);
}

void	lunch_program(char **env)
{
	char	*line;

	line = NULL;
	while (1)
	{
		set_g_data(env);
		if (ft_readline(&line))
			continue ;
		g_data.lex = lexer(line);
		if (!syntax_errors(g_data.lex))
		{
			if (parse(&g_data.tree, g_data.lex->head, g_data.ev)
				== EXIT_FAILURE)
				continue ;
			execute(g_data.tree->root, &env, cmd_count(g_data.tree->root),
				g_data.lex->head);
			free_lexer_parser(g_data.lex, g_data.tree);
			ft_wait();
		}
		else
			free_lexer(g_data.lex);
		del_env(g_data.ev);
	}
}
