/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalee <yalee@student.42.fr.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 10:23:52 by yalee             #+#    #+#             */
/*   Updated: 2023/06/23 21:25:13 by yalee            ###   ########.fr       */
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
		cleanse(node->next);
		node = node->next;
	}
	return (head);
}

void	cleanse(t_list *node)
{
	char	*temp;
	int		i;
	int		in_squote;
	int		in_dquote;

	i = 0;
	in_dquote = -1;
	in_squote = -1;
	temp = ((t_token *)node->content)->token;
	while (temp[i])
	{
		if (temp[i] == '"' && in_squote < 0)
			in_dquote *= -1;
		if (temp[i] == 39 && in_dquote < 0)
			in_squote *= -1;
		//use strjoin
	}
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

int	check_invalid(t_list *head_tokens)
{
	t_list *node;

	node = head_tokens;
	if (((t_token *)head_tokens->content)->priority == -1)
	{
		printf("Syntax error!\n");
		return (1);
	}
	while (node->next != NULL)
	{
		if (((t_token *)node->next->content)->priority == -1)
		{
			printf("Syntax error!\n");
			return (1);
		}
		if (node->next != NULL)
			node = node->next;
	}
	return (0);
}

int is_valid_lst(t_list *head_tokens, t_list *head_env)
{
	if (check_invalid(head_tokens))
		return (0);
	expand_tokens(head_tokens, head_env);
	if (check_invalid(head_tokens))
		return (0);
	return (1);
}