/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalee <yalee@student.42.fr.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 10:24:54 by yalee             #+#    #+#             */
/*   Updated: 2023/09/15 19:39:21 by yalee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env *new_env(char **splitted_env)
{
	t_env *env;

	env = malloc(sizeof(t_env));
	if (env == NULL)
	{
		// Handle memory allocation error
		return NULL;
	}

	// Assuming key and value are dynamically allocated copies
	env->key = ft_strdup(splitted_env[0]);
	env->value = ft_strdup(splitted_env[1]);

	return env;
}

t_list *env_init(char **env)
{
	t_list *head;
	t_list *node;
	int i;
	char **splitted_env;

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
	free_2dar(splitted_env);
	// lst_free_env(head);
	// system("leaks minishell");
	// exit(0);
	return (head);
}

void pr_env(t_list *head)
{
	while (head != NULL)
	{
		t_env *temp = (t_env *)head->content;
		printf("%s=%s\n", temp->key, temp->value);
		head = head->next;
	}
}