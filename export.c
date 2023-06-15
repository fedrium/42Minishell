/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalee <yalee@student.42.fr.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 17:32:10 by yalee             #+#    #+#             */
/*   Updated: 2023/06/15 23:16:58 by yalee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void export(t_list *head)
{
	int printed;
	char *smallest_key;

	smallest_key = 0;
	printed = 0;
	while (printed < ft_lstsize(head))
	{
		smallest_key = find_smallest_key(head, smallest_key);
		// printed += print_output(head, smallest_key);
		printf("%s\n", smallest_key);
		printed++;
	}
}

char	*find_smallest_key(t_list *head, char *smallest_key)
{
	t_list *node = head;
	char *to_be_returned = NULL;

	while (node)
	{
		char *current_key = ((t_env *)node->content)->key;
		if ((!smallest_key || ft_strcmp(current_key, smallest_key) > 0) &&
			(!to_be_returned || ft_strcmp(current_key, to_be_returned) < 0))
		{
			to_be_returned = current_key;
		}
		node = node->next;
	}
	return to_be_returned;
}