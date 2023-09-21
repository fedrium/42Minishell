/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalee <yalee@student.42.fr.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 10:23:52 by yalee             #+#    #+#             */
/*   Updated: 2023/09/21 16:48:49 by yalee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*tokenize(char *line, t_list *head_env)
{
	t_list	*head;
	t_list	*node;
	int		p;

	if (!line[0])
		return (ft_lstnew("\0"));
	p = 0;
	head = ft_lstnew((void *)get_token(line, &p));
	if (!check_invalid(head, 1))
		cleanse(head, head_env);
	while (line[p] == ' ')
		p++;
	node = head;
	while (line[p])
	{
		while (line[p] == ' ')
			p++;
		node->next = ft_lstnew((void *)get_token(line, &p));
		if (!check_invalid(node, 1))
			cleanse(node->next, head_env);
		while (line[p] == ' ')
			p++;
		node = node->next;
	}
	return (head);
}

void	extend(t_cleanse_vars *cleanse_vars)
{
	cleanse_vars->join[0] = cleanse_vars->temp[cleanse_vars->i];
	cleanse_vars->i++;
	cleanse_vars->new = lexer_strjoin(cleanse_vars->new, cleanse_vars->join);
}

void	cleanse(t_list *node, t_list *head_env)
{
	t_cleanse_vars	*cleanse_vars;

	cleanse_vars = init_cleanse(node);
	while (cleanse_vars->temp[cleanse_vars->i])
	{
		if (cleanse_vars->temp[cleanse_vars->i] == '$'
			&& cleanse_vars->in_squote < 0)
		{
			cleanse_vars->i++;
			expand_n_join(&cleanse_vars->temp, &cleanse_vars->new,
				&cleanse_vars->i, head_env);
		}
		if (!cleanse_vars->temp[cleanse_vars->i])
			break ;
		if (can_move(cleanse_vars->temp[cleanse_vars->i], &cleanse_vars->i,
				&cleanse_vars->in_squote, &cleanse_vars->in_dquote))
			extend(cleanse_vars);
	}
	check_quotes(cleanse_vars, node);
	free(((t_token *)node->content)->token);
	free(cleanse_vars);
	((t_token *)node->content)->token = cleanse_vars->new;
}

void	expand_n_join(char **temp, char **new, int *i, t_list *head_env)
{
	char	*key;
	char	*value;
	char	buffer[2];
	t_list	*tenv;

	buffer[1] = '\0';
	key = ft_strdup("");
	tenv = head_env;
	value = NULL;
	expansion_get_key(temp, i, buffer, &key);
	expansion_get_value(&tenv, &key, &value);
	if (ft_strncmp(key, "?", 2) == 0)
		value = ft_itoa(g_ercode);
	if (value)
	{
		(*new) = lexer_strjoin((*new), value);
		free(value);
	}
	free(key);
}
