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
	node = head;
	while (line[p])
	{
		while (line[p] == ' ')
			p++;
		node->next = ft_lstnew((void *)get_token(line, &p));
		if (!check_invalid(node, 1))
			cleanse(node->next, head_env);
		node = node->next;
	}
	return (head);
}

void cleanse(t_list *node, t_list *head_env) {
    char *temp;
    char *new;
    char join[2];
    int i;
    int in_squote;
    int in_dquote;

    i = 0;
    in_dquote = -1;
    in_squote = -1;
    temp = ((t_token *)node->content)->token;
    join[1] = '\0';
    new = ft_strdup(""); // Initialize 'new' with an empty string
    while (temp[i]) {
        if (temp[i] == '$' && in_squote < 0)
            expand_n_join(&temp, &new, &i, head_env);
        if (can_move(temp[i], &i, &in_squote, &in_dquote)) {
            join[0] = temp[i];
            i++;
            new = ft_strjoin(new, join);
        }
    }
    if (in_dquote > 0 || in_squote > 0)
        ((t_token *)node->content)->priority = -1;
    free(((t_token *)node->content)->token); // Free the old token
    ((t_token *)node->content)->token = new; // Update the token with the new value
}

int	can_move(char c, int *i, int *isq, int *idq)
{
	if (c == '"' && (*isq) < 0)
	{
		(*i)++;
		(*idq) *= -1;
		return (0);
	}
	if (c == 39 && (*idq) < 0)
	{
		(*i)++;
		(*isq) *= -1;
		return (0);
	}
	return (1);
}

void	expand_n_join(char **temp, char **new, int *i, t_list *head_env)
{
	char	*key;
	char	*value;
	// char	*temp_env;
	char	buffer[2];
	t_list	*node_env;

	node_env = head_env;
	buffer[1] = '\0';
	value = NULL;
	while ((*temp)[(*i)] != ' ' && (*temp)[(*i)] != '\0' && ((*temp)[(*i)] != '"' && (*temp)[(*i)] != 39))
	{
		key = malloc(sizeof(char));
		key[0] = '\0';
		buffer[0] = (*temp)[(*i)];
		key = ft_strjoin(key, buffer);
		(*i)++;
		free(key);
	}
	key++;
	while (node_env->next != NULL)
	{
		if (strncmp(key, ((t_env *)node_env->content)->key, ft_strlen(key) + 1) == 0)
		{
			value = ft_strdup(((t_env *)node_env->content)->value);
			break;
		}
		if (node_env->next != NULL)
			node_env = node_env->next;
	}
	if (value)
		*new = ft_strjoin(*new, value);
}

t_token	*get_token(char *line, int *p)
{
	t_token	*token;
	char	*join;
	int		quote;
	int		squote;

	quote = -1;
	squote = -1;
	join = (char *)malloc(sizeof(char) * 2);
	token = (t_token *)malloc(sizeof(t_token));
	token->token = (char *)malloc(sizeof(char));
	token->token[0] = '\0';
	join[1] = '\0';
	while (line[*p] != ' '|| quote > 0 || squote > 0)
	{
		if (!line[*p])
			break;
		if (line[*p] == 39 && quote < 0)
		{
			// printf("here\n");
			squote *= -1;
		}
		if (line[*p] == '"' && squote < 0)
		{
			// printf("here2\n");
			quote *= -1;
		}
		join[0] = line[*p];
		token->token = ft_strjoin(token->token, join);
		(*p) += 1;
	}
	// token->token[*p] = '\0';
	if (quote > 0 || squote > 0)
	{
		// token->token = NULL;
		token->priority = -1;
	}
	return (token);
}

int	check_invalid(t_list *head_tokens, int mute)
{
	t_list *node;

	node = head_tokens;
	if (((t_token *)head_tokens->content)->priority == -1)
	{
		if (!mute)
			printf("error: %s\nSyntax error!\n", ((t_token *)head_tokens->content)->token);
		return (1);
	}
	while (node->next != NULL)
	{
		if (((t_token *)node->next->content)->priority == -1)
		{
			if (!mute)
				printf("error: %s\nSyntax error!\n", ((t_token *)node->next->content)->token);
			return (1);
		}
		if (node->next != NULL)
			node = node->next;
	}
	return (0);
}
