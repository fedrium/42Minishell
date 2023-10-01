/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   misc.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalee <yalee@student.42.fr.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 16:24:40 by yalee             #+#    #+#             */
/*   Updated: 2023/10/02 00:55:11 by yalee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	lst_free_one(t_list *node)
{
	free(((t_token *)node->content)->token);
	free(node->content);
	free(node);
}

void	lst_free_env(t_list *head_env)
{
	t_list	*temp_env;
	char	*key;
	char	*value;

	while (head_env != NULL)
	{
		temp_env = head_env;
		head_env = head_env->next;
		free(((t_env *)temp_env->content)->key);
		free(((t_env *)temp_env->content)->value);
		free(temp_env->content);
		free(temp_env);
	}
}

void	free_2dar(char **ar_2d)
{
	int	i;

	i = 0;
	while (ar_2d[i])
	{
		free(ar_2d[i]);
		i++;
	}
	free(ar_2d);
}

void	lst_free_all(t_list *head)
{
	t_list	*temp;

	while (head != NULL)
	{
		temp = head;
		head = head->next;
		lst_free_one(temp);
	}
}

char	is_special(t_list *node)
{
	if (ft_strncmp(((t_token *)node->content)->token, "|", 2) == 0)
		return ('|');
	else
		return (0);
}
