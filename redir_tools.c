/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_tools.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalee <yalee@student.42.fr.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 15:36:38 by yalee             #+#    #+#             */
/*   Updated: 2023/09/21 16:33:31 by yalee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	redir_rm(t_list **head, t_list *to_del)
{
	t_list	*temp;
	t_list	*prev;

	prev = NULL;
	temp = *head;
	while (temp != NULL)
	{
		if (temp == to_del)
		{
			prev->next = temp->next;
			free(((t_token *)temp->content)->token);
			free(temp->content);
			free(temp);
			break ;
		}
		prev = temp;
		temp = temp->next;
	}
}

void	ll_iter(t_list *head_tokens)
{
	t_list	*head;

	while (head_tokens != NULL)
	{
		head = head_tokens;
		printf("line: %s\n", (((t_token *)head->content)->token));
		head_tokens = head_tokens->next;
	}
}
