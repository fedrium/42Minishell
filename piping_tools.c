/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   piping_tools.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalee <yalee@student.42.fr.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 15:32:29 by yalee             #+#    #+#             */
/*   Updated: 2023/09/22 00:07:27 by yalee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_cp(t_cp *head)
{
	t_cp	*temp_cp;

	while (head != NULL)
	{
		temp_cp = head;
		head = head->next;
		lst_free_all(temp_cp->tokens);
		free(temp_cp);
	}
}

void	split_args(t_list **segment, t_list **head_tokens)
{
	t_list	*temp;

	*segment = *head_tokens;
	while ((*head_tokens)->next != NULL
		&& is_special((*head_tokens)->next) != '|')
		(*head_tokens) = (*head_tokens)->next;
	if ((*head_tokens)->next != NULL && (*head_tokens)->next->next != NULL)
	{
		temp = (*head_tokens)->next->next;
		lst_free_one((*head_tokens)->next);
		(*head_tokens)->next = NULL;
		(*head_tokens) = temp;
		return ;
	}
	(*head_tokens) = (*head_tokens)->next;
}

int	get_cp_size(t_cp *cp)
{
	int	i;

	i = 0;
	while (cp != NULL)
	{
		cp = cp->next;
		i++;
	}
	return (i);
}

void	iterate_list(t_piping_vars **piping_vars)
{
	if ((*piping_vars)->num_processes > 1)
		close((*piping_vars)->child_processes->pipe[0]);
	if ((*piping_vars)->child_processes->next != NULL)
		close((*piping_vars)->child_processes->next->pipe[1]);
	(*piping_vars)->child_processes = (*piping_vars)->child_processes->next;
}

void	wait_cp(t_piping_vars *piping_vars)
{
	waitpid(0, &g_ercode, 0);
	g_ercode = g_ercode % 255;
	if (g_ercode == 13)
		g_ercode = 1;
	piping_vars->num_processes--;
}
