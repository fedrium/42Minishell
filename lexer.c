/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalee <yalee@student.42.fr.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 10:23:52 by yalee             #+#    #+#             */
/*   Updated: 2023/09/07 13:26:39 by yalee            ###   ########.fr       */
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
	// printf("%s, len: %li\n", temp, ft_strlen(temp));
    new = ft_strdup(""); // Initialize 'new' with an empty string
    while (temp[i])
	{
        if (temp[i] == '$' && in_squote < 0)
		{
			// printf("b4 ex: %s\n", temp);
			i++;
            expand_n_join(&temp, &new, &i, head_env);
			// printf("%s\n", temp);
		}
		if (!temp[i])
			break;
        if (can_move(temp[i], &i, &in_squote, &in_dquote))
		{
            join[0] = temp[i];
            i++;
            new = ft_strjoin(new, join);
			// printf("%c\n", temp[i]);
        }
    }
	// printf("%s\n", new);
    if (in_dquote > 0 || in_squote > 0)
	{
		printf("flag\n");
        ((t_token *)node->content)->priority = -1;
	}
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

void expand_n_join(char **temp, char **new, int *i, t_list *head_env) 
{
	char	*key;
	char	*value;
	char	buffer[2];
	t_list	*tenv;

	buffer[1] = '\0';
	key = ft_strdup("");
	tenv = head_env;
	value = NULL;
	while ((*temp)[(*i)] != 0 && ((*temp)[(*i)] != ' ' || (*temp)[(*i)] != '"' || (*temp)[(*i)] != '$'))
	{
		if ((*temp)[(*i)] == ' ' || (*temp)[(*i)] == '"' || (*temp)[(*i)] == '$')
			break;
		buffer[0] = (*temp)[(*i)];
		key = ft_strjoin(key, buffer);
		(*i)++;
	}
	while (tenv != NULL)
	{
		if (ft_strncmp(key, ((t_env *)tenv->content)->key, ft_strlen(key)) == 0)
		{
			value = ft_strdup(((t_env *)tenv->content)->value);
			break;
		}
		tenv = tenv->next;
	}
	if (value)
	{
		(*new) = ft_strjoin((*new), value);
		free(value);
	}
	free(key);
}

t_token	*get_token(char *line, int *p)
{
	t_token	*token;
	char	*join;
	int		quote;
	int		squote;

	quote = -1;
	squote = -1;
	token = (t_token *)malloc(sizeof(t_token));
	join = (char *)malloc(sizeof(char) * 2);
	token->token = (char *)malloc(sizeof(char));
	token->token[0] = '\0';
	join[1] = '\0';
	while (line[*p] != ' '|| quote > 0 || squote > 0)
	{
		if (!line[*p])
			break;
		if (line[*p] == 39 && quote < 0)
			squote *= -1;
		if (line[*p] == '"' && squote < 0)
			quote *= -1;
		if ((squote < 0 || quote < 0) && line[(*p) + 1] == '|')
			token->priority = -1;
		join[0] = line[*p];
		token->token = ft_strjoin(token->token, join);
		(*p) += 1;
	}
	if (quote > 0 || squote > 0)
		token->priority = -1;
	return (token);
}

int	check_invalid(t_list *head_tokens, int mute)
{
	t_list *node;

	node = head_tokens;
	while (node != NULL)
	{
		if (((t_token *)node->content)->priority == -1)
		{
			if (!mute)
				printf("error: %s\nSyntax error!\n", ((t_token *)node->content)->token);
			return (1);
		}
		node = node->next;
	}
	return (0);
}

void	check_head_tokens(t_list *node, char *line)
{
	if (line[0] == '\0')
		return;
	if (line[0] == '|')
		((t_token *)node->content)->priority = -1;
	if (ft_strncmp(get_tl_str(node), "|", 2) == 0)
		((t_token *)node->content)->priority = -1;
	if (ft_strncmp(get_tl_str(node), ">", 2) == 0)
		((t_token *)node->content)->priority = -1;
	if (ft_strncmp(get_tl_str(node), "<", 2) == 0)
		((t_token *)node->content)->priority = -1;
	if (ft_strncmp(get_tl_str(node), ">>", 3) == 0)
		((t_token *)node->content)->priority = -1;
	if (ft_strncmp(get_tl_str(node), "<<", 3) == 0)
		((t_token *)node->content)->priority = -1;
	if (ft_strncmp(get_tl_str(node), "&", 2) == 0)
		((t_token *)node->content)->priority = -1;
	if (ft_strncmp(get_tl_str(node), "||", 3) == 0)
		((t_token *)node->content)->priority = -1;
	if (ft_strncmp(get_tl_str(node), "&&", 3) == 0)
		((t_token *)node->content)->priority = -1;
}