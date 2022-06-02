/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_search.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-khni <ael-khni@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/08 14:46:20 by fathjami          #+#    #+#             */
/*   Updated: 2022/05/25 08:27:25 by ael-khni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// TODO: free itoa
char	*get_env_value(char *key, t_env *env)
{
	t_env_elem	*elem;

	if (ft_strcmp(key, "?") == EQUAL || ft_strcmp(key, "?\n") == EQUAL)
		return (ft_itoa(g_data.exit_status));
	elem = search_env_elem(env, key);
	if (!elem)
		return (NULL);
	else
		return (ft_strdup(elem->value));
}

t_env_elem	*search_env_elem(t_env *env, char *key)
{
	t_env_elem	*tmp;

	tmp = env->head;
	while (tmp && strcmp(tmp->key, key))
		tmp = tmp->next;
	return (tmp);
}

char	*search_env_var(char *key, char **env)
{
	int		i;
	int		pos;
	char	*var;

	i = 0;
	if (ft_strcmp(key, "?") == EQUAL)
		return (ft_itoa(g_data.exit_status));
	while (env[i] && ft_strncmp(env[i], key, ft_strlen(key)))
		i++;
	if (!env[i])
		return (NULL);
	pos = ft_strchr(env[i], '=');
	if (pos == -1)
		return (NULL);
	var = ft_substr(env[i], pos + 1, ft_strlen(env[i]) - (pos + 1));
	return (var);
}

char	*get_expanded_value(char **line, char **env)
{
	int		key_len;
	char	key[500];

	key_len = 0;
	(*line)++;
	if (**line == '?')
		key[key_len++] = *((*line)++);
	else
	{
		while (**line && is_alphanum(**line))
			key[key_len++] = *((*line)++);
	}
	key[key_len] = '\0';
	return (search_env_var(key, env));
}

char	*expand_env(char *line, char **env)
{
	int		i;
	int		j;
	char	*value;
	char	res[1024];

	i = -1;
	value = NULL;
	while (line[++i] && line[i] != '$')
		res[i] = line[i];
	if (line[i] == '$')
	{
		line = line + i;
		value = get_expanded_value(&line, env);
		if (value)
		{
			j = 0;
			while (value[j])
				res[i++] = value[j++];
		}
	}
	while (*line)
		res[i++] = *(line++);
	res[i] = '\0';
	return (ft_strdup(res));
}
