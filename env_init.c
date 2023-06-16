/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyu-xian <cyu-xian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/16 12:46:57 by cyu-xian          #+#    #+#             */
/*   Updated: 2023/06/16 12:47:47 by cyu-xian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*new_env(char	**splitted_env)
{
	t_env	*env;

	env = malloc(sizeof(t_env));
	env->key = ft_strdup(splitted_env[0]);
	env->value = ft_strdup(splitted_env[1]);
	return (env);
}

t_list	*env_init(char	**env)
{
	t_list	*head;
	t_list	*node;
	int		i;
	char	**splitted_env;

	i = 1;
	splitted_env = ft_split(env[0], '=');
	node = ft_lstnew((void *)new_env(splitted_env));
	head = node;
	while (env[i])
	{
		splitted_env = ft_split(env[i], '=');
		node->next = ft_lstnew((void *)new_env(splitted_env));
		node = node->next;
		i++;
	}
	return (head);
}