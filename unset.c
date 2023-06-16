/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyu-xian <cyu-xian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/16 14:04:14 by cyu-xian          #+#    #+#             */
/*   Updated: 2023/06/16 21:58:46 by cyu-xian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	unset(char *line, t_list *head)
{
	char	**array;
	t_list	*current;

	array = ft_split(line, ' ');
	while (head->next != NULL)
	{
		t_env *temp = (t_env *)head->content;
		if (ft_strncmp(array[1], temp->key, ft_strlen(temp->key) + 1) == 0)
		{
			free(temp->key);
			free(temp->value);
			free(temp);
		}
		head = head->next;
	}
}