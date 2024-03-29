/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyu-xian <cyu-xian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 10:23:52 by yalee             #+#    #+#             */
/*   Updated: 2023/09/28 17:24:22 by cyu-xian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	rm_quote(t_list *node, t_list *head_env)
{
	t_list	*head;

	head = node;
	while (head != NULL)
	{
		if (!check_invalid(head, 1))
			cleanse(head, head_env);
		head = head->next;
	}
}

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
	while (line2[p] == ' ')
		p++;
	node = head;
	while (line2[p])
	{
		while (line2[p] == ' ')
			p++;
		node->next = ft_lstnew((void *)get_token(&line2, &p, head_env));
		while (line2[p] == ' ')
			p++;
		node = node->next;
	}
	free (line2);
	rm_quote(head, head_env);
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

void	copy_b4_meta(char **line, int *p)
{
	char	*temp;
	int		i;

	i = 0;
	temp = ft_strdup(*line);
	free(*line);
	while (temp[i] != '$')
		i++;
	*line = malloc(sizeof(char) * (i + 1));
	i = 0;
	while (temp[i] != '$')
	{
		(*line)[i] = temp[i];
		i++;
	}
	(*line)[i] = '\0';
	free (temp);
}
