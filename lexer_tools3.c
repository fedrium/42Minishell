/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_tools3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalee <yalee@student.42.fr.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 15:30:01 by yalee             #+#    #+#             */
/*   Updated: 2023/09/26 17:06:50 by yalee            ###   ########.fr       */
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

t_token	*get_token(char *line, int *p, t_env *env)
{
	t_token	*token;
	char	join[2];
	int		quote;
	int		squote;

	init_get_token(&quote, &squote, &token, &join);
	while (line[*p] != ' ' || quote > 0 || squote > 0)
	{
		if (!line[*p])
			break ;
		if (line[*p] == 39 && quote < 0)
			squote *= -1;
		if (line[*p] == '"' && squote < 0)
			quote *= -1;
		if (line[*p] == '$' && squote < 0)
			expand_n_join(&line, p, quote);
		join[0] = line[*p];
		token->token = lexer_strjoin(token->token, join);
		(*p) += 1;
	}
	if (quote > 0 || squote > 0)
		token->priority = -1;
	return (token);
}
