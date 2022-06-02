/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-khni <ael-khni@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/08 14:07:48 by fathjami          #+#    #+#             */
/*   Updated: 2022/05/28 13:50:01 by ael-khni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*parse_quotes(char *str, t_elem **ptr, enum e_token type, t_env *env)
{
	char	*value;

	*ptr = (*ptr)->next;
	while (*ptr && (*ptr)->type != type)
	{
		if (!str)
			str = ft_strdup("");
		if ((*ptr)->type == ENV && (*ptr)->state == IN_DQUOTE)
		{
			value = get_env_value((*ptr)->content + 1, env);
			if (value)
				str = ft_strjoin(str, value);
		}
		else
			str = ft_strjoin(str, (*ptr)->content);
		*ptr = (*ptr)->next;
	}
	if (*ptr)
		*ptr = (*ptr)->next;
	return (str);
}

t_redir_list	*parse_here_doc(t_redir_list *redir, char *arg, char **env)
{	
	char	*line;
	int		fd;

	fd = open("/tmp/.minishell_tmp",
			O_RDWR | O_CREAT | O_APPEND | O_TRUNC, 0666);
	line = get_next_line(STDIN_FILENO);
	while (line)
	{
		if (ft_strncmp(line, arg, ft_strlen(arg)) == 0)
			break ;
		else
		{
			if (ft_strchr(line, '$') != -1)
				line = expand_env(line, env);
			write(fd, line, ft_strlen(line));
		}
		free(line);
		line = get_next_line(STDIN_FILENO);
	}
	free(arg);
	add_redir_tail(redir,
		new_redir_elem(ft_strdup("/tmp/.minishell_tmp"), HERE_DOC));
	close(fd);
	return (redir);
}

t_redir_list	*parse_redir(t_elem **ptr, t_redir_list *redir, t_env *env)
{
	enum e_token	type;
	char			*arg;

	type = (*ptr)->type;
	while ((*ptr)->type != WORD && (*ptr)->type != ENV)
		*ptr = (*ptr)->next;
	if ((*ptr)->type == ENV && type != HERE_DOC)
	{
		arg = get_env_value((*ptr)->content + 1, env);
		if (!arg)
		{
			printf("bash: %s: ambiguous redirect\n", (*ptr)->content);
			return (NULL);
		}
	}
	else
		arg = ft_strdup((*ptr)->content);
	*ptr = (*ptr)->next;
	if (type == HERE_DOC)
		return (parse_here_doc(redir, arg, env->env));
	add_redir_tail(redir, new_redir_elem(arg, type));
	return (redir);
}

// TODO: freee
t_ast_node	*parse_pipe(t_elem **ptr)
{
	t_ast_node	*node;

	node = ft_calloc(sizeof(t_ast_node), 1);
	if (!node)
		return (NULL);
	node->type = PIPE;
	node->content = malloc(sizeof(t_union));
	node->content->pipe = malloc(sizeof(t_pipe));
	node->content->pipe->left = ft_calloc(sizeof(t_ast_node), 1);
	node->content->pipe->right = ft_calloc(sizeof(t_ast_node), 1);
	if (!node->content->pipe->right || !node->content->pipe->left)
		return (NULL);
	*ptr = (*ptr)->next;
	while ((*ptr)->type == WHITE_SPACE)
		*ptr = (*ptr)->next;
	return (node);
}

void	parse_word(t_elem **ptr, char **args, int *i)
{
	args[*i] = ft_strndup((*ptr)->content, (*ptr)->len);
	(*i)++;
	(*ptr) = (*ptr)->next;
}
