/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalee <yalee@student.42.fr.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 17:32:10 by yalee             #+#    #+#             */
/*   Updated: 2023/06/20 02:00:32 by yalee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void export(t_list *head_env, t_list *head_tokens)
{
	int printed;
	char *smallest_key;

	if (head_tokens->next == NULL)
	{
		smallest_key = 0;
		printed = 0;
		while (printed < ft_lstsize(head_env))
		{
			smallest_key = find_smallest_key(head_env, smallest_key);
			printed += print_output(head_env, smallest_key);
		}
	}
// 	else
// 	{
		
// 	}
// }

char	*find_smallest_key(t_list *head, char *smallest_key)
{
	t_list *node = head;
	char *to_be_returned = NULL;

	while (node)
	{
		char *current_key = ((t_env *)node->content)->key;
		if ((!smallest_key || ft_strcmp(current_key, smallest_key) > 0) &&
			(!to_be_returned || ft_strcmp(current_key, to_be_returned) < 0))
			to_be_returned = current_key;
		node = node->next;
	}
	return (to_be_returned);
}

int	print_output(t_list	*head, char *smallest_key)
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
			printf("declare -x %s=\"%s\"\n", ((t_env *)node->content)->key, ((t_env *)node->content)->value);
			printed++;
		}
		i++;
		if (node->next != NULL)
			node = node->next;
	}
	return (printed);
}