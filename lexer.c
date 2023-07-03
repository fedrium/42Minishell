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
	while (line[p] == ' ')
		p++;
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

int	morph_cacoon(char *cacoon, t_list *head_env, t_list *node)
{
	t_list	*env;
	t_env	*temp;
	int		i;
	int		lst_len;
	
	env = head_env;
	i = 0;
	lst_len = ft_lstsize(env);
	cacoon++;
	// pr_env(env);
	while (i < lst_len)
	{
		temp = (t_env *)env->content;
		if (ft_strncmp(cacoon, temp->key, (ft_strlen(cacoon) + 1)) == 0)
		{
			free(((t_token *)node->content)->token);
			((t_token *)node->content)->token = ft_strdup(temp->value);
			return (1);
		}
		env = env->next;
		i++;
	}
	return (0);
}

int	morph(t_list *node, t_list *head_env)
{
	int		i;
	char	*cacoon;
	char	key[2];

	i = 0;
	key[1] = '\0';
	cacoon = (char *)malloc(sizeof(char));
	cacoon[0] = '\0';
	while (((t_token *)node->content)->token[i] != '$' && ((t_token *)node->content)->token[i])
		i++;
	if (((t_token *)node->content)->token[i] == '\0')
		return (1);
	while (((t_token *)node->content)->token[i] && ((t_token *)node->content)->token[i] != '"')
	{
		key[0] = ((t_token *)node->content)->token[i];
		cacoon = ft_strjoin(cacoon, key);
		i++;
	}
	if (!morph_cacoon(cacoon, head_env, node))
	{
		free(cacoon);
		return (0);
	}
	free(cacoon);
	return (1);
}

void expand_tokens(t_list *head_tokens, t_list *head_env)
{
	t_list *node;
	int i;
	int size;

	node = head_tokens;
	i = 0;
	size = ft_lstsize(head_tokens);
	while (i < size)
	{
		morph(node, head_env);
		node = node->next;
		i++;
	}
	return;
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