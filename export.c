/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalee <yalee@student.42.fr.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 17:32:10 by yalee             #+#    #+#             */
/*   Updated: 2023/06/23 01:25:01 by yalee            ###   ########.fr       */
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
	// else
	// 	add_env(head_env, head_tokens);
}

// void	add_env(t_list *head_env, t_list *head_tokens)
// {
// 	t_list	*node_e;
// 	t_list	*node_t;
// 	int		i;
// 	int		j;

// 	i = 0;
// 	node_t = head_tokens->next;
// 	node_e = head_env;
// 	while (i < ft_lstsize(node_t))
// 	{
// 		j = 0;
// 		node_e = head_env;
// 		while (j < ft_lstsize(head_env))
// 		{
// 			if (same_key(node_t, node_e))
// 				add_env_value(node_t, node_e);
// 			node_e = node_e->next;
// 			j++;
// 		}
// 		add_env_key(ft_lstlast(node_e), node_t);
// 		add_env_value(ft_lstlast(node_e), node_t);
// 		node_t = node_t->next;
// 		i++;
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