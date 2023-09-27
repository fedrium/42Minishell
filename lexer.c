/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalee <yalee@student.42.fr.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 10:23:52 by yalee             #+#    #+#             */
/*   Updated: 2023/09/27 21:35:44 by yalee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*tokenize(char *line, t_list *head_env)
{
	t_list	*head;
	t_list	*node;
	char	*line2;
	int		p;

	if (!line[0])
		return (ft_lstnew("\0"));
	p = 0;
	line2 = ft_strdup(line);
	head = ft_lstnew((void *)get_token(&line2, &p, head_env));
	if (!check_invalid(head, 1))
		cleanse(head, head_env);
	// printf("token: %s\n", ((t_token *)head->content)->token);
	// printf("line after get_token: %s\n", line2);
	while (line2[p] == ' ')
		p++;
	node = head;
	while (line2[p])
	{
		while (line2[p] == ' ')
			p++;
		node->next = ft_lstnew((void *)get_token(&line2, &p, head_env));
		if (!check_invalid(node, 1))
			cleanse(node->next, head_env);
		while (line2[p] == ' ')
			p++;
		// printf("token: %s\n", ((t_token *)node->next->content)->token);
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

char	*exp_get_key(char *line, int p, int quote)
{
	char	*key;
	char	buffer[2];

	p++;
	buffer[1] = '\0';
	key = ft_strdup("");
	while (line[p] != '$' && (line[p] != ' ' && quote < 0) && line[p])
	{
		buffer[0] = line[p];
		key = lexer_strjoin(key, buffer);
		(p)++;
	}
	return (key);
}

char	*exp_get_value(char *key, t_list *env)
{
	t_list	*head_env;

	head_env = env;
	while(head_env != NULL)
	{
		if (ft_strncmp(key, ((t_env *)head_env->content)->key,
				ft_strlen(key) + 1) == 0)
			return (ft_strdup(((t_env *)head_env->content)->value));
		head_env = head_env->next;
	}
	return (NULL);
}

void	copy_b4_meta(char **line, int *p)
{
	char	*temp;
	int		i;

	i = 0;
	temp = ft_strdup(*line);
	free(*line);
	while(temp[i] != '$')
		i++;
	*line = malloc(sizeof(char) * (i + 1));
	i = 0;
	while(temp[i] != '$')
	{
		(*line)[i] = temp[i];
		i++;
	}
	(*line)[i] = '\0';
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
	while(temp[i])
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
	// printf("P: %d\n", *p);
	// (*p) = (*p) + j;
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
