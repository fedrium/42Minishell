/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_tools.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalee <yalee@student.42.fr.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 15:25:22 by yalee             #+#    #+#             */
/*   Updated: 2023/09/21 16:02:50 by yalee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_smallest_key(t_list *head, char *smallest_key)
{
	t_list	*node;
	char	*to_be_returned;
	char	*current_key;

	to_be_returned = NULL;
	node = head;
	while (node)
	{
		current_key = ((t_env *)node->content)->key;
		if ((!smallest_key || ft_strcmp(current_key, smallest_key) > 0)
			&& (!to_be_returned || ft_strcmp(current_key, to_be_returned) < 0))
			to_be_returned = current_key;
		node = node->next;
	}
	return (to_be_returned);
}

int	print_output(t_list *head, char *smallest_key)
{
	int		printed;
	int		i;
	t_list	*node;

	node = head;
	printed = 0;
	i = 0;
	while (i < ft_lstsize(head))
	{
		if (ft_strcmp(((t_env *)node->content)->key, smallest_key) == 0)
		{
			printf("declare -x %s=\"%s\"\n", ((t_env *)node->content)->key,
				((t_env *)node->content)->value);
			printed++;
		}
		i++;
		if (node->next != NULL)
			node = node->next;
	}
	return (printed);
}
