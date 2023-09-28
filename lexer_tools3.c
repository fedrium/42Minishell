/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_tools3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalee <yalee@student.42.fr.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 15:30:01 by yalee             #+#    #+#             */
/*   Updated: 2023/09/28 16:15:48 by yalee            ###   ########.fr       */
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

t_cleanse_vars	*init_cleanse(t_list *node)
{
	t_cleanse_vars	*cleanse_vars;

	cleanse_vars = malloc(sizeof(t_cleanse_vars));
	cleanse_vars->i = 0;
	cleanse_vars->in_dquote = -1;
	cleanse_vars->in_squote = -1;
	cleanse_vars->temp = ((t_token *)node->content)->token;
	cleanse_vars->join[1] = '\0';
	cleanse_vars->new = ft_strdup("");
	return (cleanse_vars);
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

void	copy_after_meta(char **line, int *p, char *value)
{
	char	*temp;
	int		i;
	int		j;

	i = 0;
	j = 0;
	temp = ft_strdup((*line));
	free((*line));
	*line = malloc(sizeof(char) * (ft_strlen(temp) + ft_strlen(value) + 1));
	while (temp[i])
	{
		(*line)[i] = temp[i];
		i++;
	}
	(*line)[i] = 0;
	while (value[j])
	{
		(*line)[i] = value[j];
		i++;
		j++;
	}
	(*line)[i] = 0;
}

void	expand_n_join(char **line, int *p, int quote, t_list *env)
{
	int		new_start;
	char	*key;
	char	*value;

	key = exp_get_key(*line, *p, quote);
	value = exp_get_value(key, env);
	if (!value)
		return ;
	copy_b4_meta(line, p);
	copy_after_meta(line, p, value);
}
