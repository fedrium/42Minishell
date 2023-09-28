/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_tools3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyu-xian <cyu-xian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 15:30:01 by yalee             #+#    #+#             */
/*   Updated: 2023/09/28 19:32:13 by cyu-xian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_get_token(int *quote, int *squote, t_token **token,
	char (*join)[2])
{
	*quote = -1;
	*squote = -1;
	(*token) = malloc(sizeof(t_token));
	(*token)->token = (char *)malloc(sizeof(char));
	(*token)->token[0] = '\0';
}

t_token	*get_token(char **line, int *p, t_list *env)
{
	t_token	*token;
	char	join[2];
	int		quote;
	int		squote;

	init_get_token(&quote, &squote, &token, &join);
	while ((*line)[*p] != ' ' || quote > 0 || squote > 0)
	{
		if (!(*line)[*p])
			break ;
		if ((*line)[*p] == 39 && quote < 0)
			squote *= -1;
		if ((*line)[*p] == '"' && squote < 0)
			quote *= -1;
		if ((*line)[*p] == '$' && squote < 0)
			expand_n_join(&(*line), p, quote, env);
		join[0] = (*line)[*p];
		token->token = lexer_strjoin(token->token, join);
		(*p) += 1;
	}
	if (quote > 0 || squote > 0)
		token->priority = -1;
	return (token);
}

void	copy_after_meta(char **line, int *p, char *value, int *after)
{
	char	*temp;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (!value)
		return ;
	temp = ft_strdup((*line));
	free((*line));
	*line = malloc(sizeof(char) * (ft_strlen(temp) + ft_strlen(value) + 1));
	while (temp[i])
	{
		(*line)[i] = temp[i];
		i++;
	}
	while (value[j])
	{
		(*line)[i] = value[j];
		i++;
		j++;
	}
	free (temp);
	(*line)[i] = 0;
	(*after) = (*p) + ft_strlen(value);
}

void	copy_remaining(char **line, char *ori, int after, int marker)
{
	char	*temp;
	int		remaining;
	int		i;

	remaining = marker;
	while (ori[remaining])
		remaining++;
	temp = ft_strdup((*line));
	free ((*line));
	(*line) = malloc(sizeof(char) * (after + remaining + 1));
	i = 0;
	while (temp[i])
	{
		(*line)[i] = temp[i];
		i++;
	}
	while (ori[marker])
	{
		(*line)[i] = ori[marker];
		i++;
		marker++;
	}
	(*line)[i] = 0;
	free(temp);
}

void	expand_n_join(char **line, int *p, int quote, t_list *env)
{
	int		new_start;
	char	*key;
	char	*value;
	char	*ori;
	int		after;

	ori = ft_strdup((*line));
	key = exp_get_key(*line, *p, quote);
	value = exp_get_value(key, env);
	copy_b4_meta(line, p);
	copy_after_meta(line, p, value, &after);
	copy_remaining(line, ori, after, (*p + ft_strlen(key) + 1));
	free (key);
	free (value);
	free (ori);
}
