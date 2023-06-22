/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalee <yalee@student.42.fr.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 10:23:52 by yalee             #+#    #+#             */
/*   Updated: 2023/06/22 17:44:18 by yalee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*tokenize(char *line)
{
	t_list	*head;
	t_list	*node;
	int		p;
	
	if (!line[0])
		return (ft_lstnew("\0"));
	p = 0;
	head = ft_lstnew((void *)get_token(line, &p));
	node = head;
	while (line[p])
	{
		while (line[p] == ' ')
			p++;
		node->next = ft_lstnew((void *)get_token(line, &p));
		node = node->next;
	}
	return (head);
}

t_token	*get_token(char *line, int *p)
{
	t_token	*token;
	char	*join;
	int		quote;

	quote = -1;
	join = (char *)malloc(sizeof(char) * 2);
	token = (t_token *)malloc(sizeof(t_token));
	token->token = (char *)malloc(sizeof(char));
	token->token[0] = '\0';
	join[1] = '\0';
	while (line[*p] != ' '|| quote > 0)
	{
		if (!line[*p])
			break;
		if (line[*p] == 39 || line[*p] == '"')
			quote *= -1;
		join[0] = line[*p];
		token->token = ft_strjoin(token->token, join);
		(*p) += 1;
	}
	token->token[*p] = '\0';
	if (quote > 0)
	{
		token->token = NULL;
		token->priority = -1;
	}
	return (token);
}
