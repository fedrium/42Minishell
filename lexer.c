/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalee <yalee@student.42.fr.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 10:23:52 by yalee             #+#    #+#             */
/*   Updated: 2023/09/20 19:18:46 by yalee            ###   ########.fr       */
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

char	*lexer_strjoin(char *str1, char *str2)
{
	char	*temp1;
	char	*temp2;
	char	*new;

	temp1 = ft_strdup(str1);
	free(str1);
	temp2 = ft_strdup(str2);
	// free(str2);
	new = ft_strjoin(temp1, temp2);
	free(temp1);
	free(temp2);
	return(new);
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

void	check_quotes(t_cleanse_vars *cleanse_vars, t_list *node)
{
	if (cleanse_vars->in_dquote > 0 || cleanse_vars->in_squote > 0)
		((t_token *)node->content)->priority = -1;
}

void cleanse(t_list *node, t_list *head_env)
{
	t_cleanse_vars	*cleanse_vars;

	cleanse_vars = init_cleanse(node);
	while (cleanse_vars->temp[cleanse_vars->i])
	{
		if (cleanse_vars->temp[cleanse_vars->i] == '$' && cleanse_vars->in_squote < 0)
		{
			cleanse_vars->i++;
			expand_n_join(&cleanse_vars->temp, &cleanse_vars->new, &cleanse_vars->i, head_env);
		}
		if (!cleanse_vars->temp[cleanse_vars->i])
			break;
		if (can_move(cleanse_vars->temp[cleanse_vars->i], &cleanse_vars->i, &cleanse_vars->in_squote, &cleanse_vars->in_dquote))
		{
			cleanse_vars->join[0] = cleanse_vars->temp[cleanse_vars->i];
			cleanse_vars->i++;
			cleanse_vars->new = lexer_strjoin(cleanse_vars->new, cleanse_vars->join);
		}
    }
	check_quotes(cleanse_vars, node);
	free(((t_token *)node->content)->token);
	free(cleanse_vars);
	((t_token *)node->content)->token = cleanse_vars->new;
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

void	expansion_get_key(char **temp, int *i, char buffer[2], char **key)
{
	while ((*temp)[(*i)] != 0 && ((*temp)[(*i)] != ' ' || (*temp)[(*i)] != '"' || (*temp)[(*i)] != '$'))
	{
		if ((*temp)[(*i)] == ' ' || (*temp)[(*i)] == '"' || (*temp)[(*i)] == '$')
			break;
		buffer[0] = (*temp)[(*i)];
		(*key) = lexer_strjoin((*key), buffer);
		(*i)++;
	}
}

void	expansion_get_value(t_list **tenv, char **key, char **value)
{
	while ((*tenv) != NULL)
	{
		if (ft_strncmp((*key), ((t_env *)(*tenv)->content)->key, ft_strlen((*key))) == 0)
		{
			(*value) = ft_strdup(((t_env *)(*tenv)->content)->value);
			break;
		}
		(*tenv) = (*tenv)->next;
	}
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

void	init_get_token(int *quote, int *squote, t_token **token, char (*join)[2])
{
	*quote = -1;
	*squote = -1;
	(*token) = malloc(sizeof(t_token));
	(*token)->token = (char *)malloc(sizeof(char));
	(*token)->token[0] = '\0';
}

t_token	*get_token(char *line, int *p)
{
	t_token	*token;
	char	join[2];
	int		quote;
	int		squote;

	init_get_token(&quote, &squote, &token, &join);
	while (line[*p] != ' '|| quote > 0 || squote > 0)
	{
		if (!line[*p])
			break;
		if (line[*p] == 39 && quote < 0)
			squote *= -1;
		if (line[*p] == '"' && squote < 0)
			quote *= -1;
		join[0] = line[*p];
		token->token = lexer_strjoin(token->token, join);
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