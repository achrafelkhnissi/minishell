/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fathjami <fathjami@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/23 18:18:31 by ael-khni          #+#    #+#             */
/*   Updated: 2022/05/26 15:14:29 by fathjami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_isspace(char c)
{
	if (c == ' ' || c == '\t' || c == '\v'
		|| c == '\r' || c == '\f' || c == '\n')
		return (1);
	return (0);
}

int	in_charset(char c)
{
	return (c == '\'' || c == '\"' || c == '<' || c == '>' || c == '|'
		|| c == '$' || ft_isspace(c) || c == '\n' || c == '\0');
}

int	get_word(t_list *tokens, char *str, enum e_state state)
{
	int	i;

	i = 0;
	while (!in_charset(str[i]))
		i++;
	add_tail(tokens, new_elem(str, i, WORD, state));
	return (i);
}

int	get_env_var(t_list *tokens, char *str, enum e_state state)
{
	int	i;

	i = 1;
	if (str[i] == '?' || (str[i] >= '0' && str[i] <= '9'))
		i++;
	else
		while (is_alphanum(str[i]) && str[i] != '\n' && str[i] != '\0')
			i++;
	add_tail(tokens, new_elem(str, i, ENV, state));
	return (i);
}

void	quotes_state(t_list *tokens, char *str, enum e_state *state, int flag)
{
	enum e_state	e_state;
	enum e_token	e_type;

	if (flag == 1)
	{
		e_state = IN_QUOTE;
		e_type = QOUTE;
	}
	else
	{
		e_state = IN_DQUOTE;
		e_type = DOUBLE_QUOTE;
	}
	if (*state == GENERAL)
	{
		add_tail(tokens, new_elem(str, 1, e_type, *state));
		*state = e_state;
	}
	else if (*state == e_state)
	{
		*state = GENERAL;
		add_tail(tokens, new_elem(str, 1, e_type, *state));
	}
	else
		add_tail(tokens, new_elem(str, 1, e_type, *state));
}
