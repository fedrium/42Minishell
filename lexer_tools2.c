/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_tools2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalee <yalee@student.42.fr.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 15:27:47 by yalee             #+#    #+#             */
/*   Updated: 2023/10/02 00:57:47 by yalee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_invalid(t_list *head_tokens, int mute)
{
	t_list	*node;

	node = head_tokens;
	while (node != NULL)
	{
		if (((t_token *)node->content)->priority == -1)
		{
			if (!mute)
				printf("error: %s\nSyntax error!\n",
					((t_token *)node->content)->token);
			return (1);
		}
		node = node->next;
	}
	return (0);
}

void	check_head_tokens(t_list *node, char *line)
{
	if (line[0] == '\0')
		return ;
	if (line[0] == '|')
		((t_token *)node->content)->priority = -1;
	if (ft_strncmp(get_tl_str(node), "|", 2) == 0)
		((t_token *)node->content)->priority = -1;
	if (ft_strncmp(get_tl_str(node), ">", 2) == 0)
		((t_token *)node->content)->priority = -1;
	if (ft_strncmp(get_tl_str(node), ">>", 3) == 0)
		((t_token *)node->content)->priority = -1;
	if (ft_strncmp(get_tl_str(node), "&", 2) == 0)
		((t_token *)node->content)->priority = -1;
	if (ft_strncmp(get_tl_str(node), "||", 3) == 0)
		((t_token *)node->content)->priority = -1;
	if (ft_strncmp(get_tl_str(node), "&&", 3) == 0)
		((t_token *)node->content)->priority = -1;
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

void	check_quotes(t_cleanse_vars *cleanse_vars, t_list *node)
{
	if (cleanse_vars->in_dquote > 0 || cleanse_vars->in_squote > 0)
		((t_token *)node->content)->priority = -1;
}

int	is_last_arg(t_list *head_tokens)
{
	t_list	*p;

	p = head_tokens;
	while (p != NULL)
	{
		printf("%s\n", ((t_token *)p->content)->token);
		if (ft_strncmp(((t_token *)p->content)->token, "|", 1) == 0
			&& p->next != NULL)
			return (0);
		p = p->next;
	}
	return (1);
}
