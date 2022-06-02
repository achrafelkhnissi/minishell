/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_core.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fathjami <fathjami@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/08 17:08:21 by fathjami          #+#    #+#             */
/*   Updated: 2022/05/30 11:50:13 by fathjami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	env_act(t_elem **ptr, t_env *env, char **args, int *i)
{
	args[*i] = get_env_value((*ptr)->content + 1, env);
	if (args[*i])
		(*i)++;
	(*ptr) = (*ptr)->next;
}

t_redir_list	*redir_act(t_elem **ptr, t_env *env, t_redir_list *redir)
{
	return (parse_redir(ptr, redir, env));
}

int	parse_cmd_act(t_elem **ptr, t_env *env,
		t_redir_list *redir, char **args)
{
	int	i;

	i = 0;
	while ((*ptr) && (*ptr)->type != PIPE_LINE)
	{
		if ((*ptr)->type == WORD)
			parse_word(ptr, args, &i);
		else if ((*ptr)->type == WHITE_SPACE)
			(*ptr) = (*ptr)->next;
		else if ((*ptr)->type == DOUBLE_QUOTE || (*ptr)->type == QOUTE)
		{
			args[i] = parse_quotes(args[i], ptr, (*ptr)->type, env);
			if (args[i])
				(i)++;
		}
		else if ((*ptr)->type == ENV)
			env_act(ptr, env, args, &i);
		else if (is_redir((*ptr)->type))
		{
			if (!redir_act(ptr, env, redir))
				return (EXIT_FAILURE);
		}
	}
	args[i] = NULL;
	return (EXIT_SUCCESS);
}

t_ast_node	*parse_cmd(t_elem **ptr, t_env *env)
{
	t_ast_node		*new;
	char			**args;
	t_redir_list	*redir;
	int				size;
	int				i;

	i = 0;
	redir = NULL;
	size = count_args((*ptr));
	args = ft_calloc(sizeof(char *), (size + 1));
	redir = init_redir_list(redir);
	if (!args || !redir)
		return (NULL);
	if (parse_cmd_act(ptr, env, redir, args) == EXIT_FAILURE)
		return (NULL);
	new = create_cmd_node(args, env->env, redir);
	return (new);
}

int	parse(t_ast **tree, t_elem *ptr, t_env *env)
{
	t_ast_node	*tmp_node;

	tmp_node = NULL;
	if (!ptr)
		return (EXIT_FAILURE);
	tmp_node = parse_cmd(&ptr, env);
	if (!tmp_node)
		return (EXIT_FAILURE);
	if (!(*tree)->root)
		(*tree)->root = tmp_node;
	else
	{
		if (!(*tree)->root->content->pipe->right)
			(*tree)->root->content->pipe->right = tmp_node;
		else
			(*tree)->root->content->pipe->left = tmp_node;
	}
	if (ptr && ptr->type == PIPE_LINE)
	{
		tmp_node = parse_pipe(&ptr);
		tmp_node->content->pipe->right = (*tree)->root;
		(*tree)->root = tmp_node;
	}
	parse(tree, ptr, env);
	return (EXIT_SUCCESS);
}
