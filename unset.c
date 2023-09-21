/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalee <yalee@student.42.fr.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 15:17:46 by yalee             #+#    #+#             */
/*   Updated: 2023/09/21 16:53:05 by yalee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_unset(t_list *temp_env)
{
	free(((t_env *)temp_env->content)->key);
	free(((t_env *)temp_env->content)->value);
	free(temp_env->content);
	free(temp_env);
}

void	init_unset(t_list **head_env, t_list **prev_env,
	t_list **slave, t_list **env)
{
	*head_env = *env;
	*prev_env = ft_lstnew(NULL);
	*slave = *prev_env;
	(*prev_env)->next = *head_env;
}

void	mod_env(t_list **temp_env, t_list **head_env, t_list **prev_env)
{
	(*temp_env) = (*head_env);
	(*head_env) = (*head_env)->next;
	if ((*prev_env)->content != NULL)
		(*prev_env)->next = (*head_env);
}

void	unset(t_list **env, t_list *token, int size)
{
	t_list	*temp_env;
	t_list	*prev_env;
	t_list	*head_env;
	t_list	*slave;
	char	*line;

	if (size == 1)
		return ;
	init_unset(&head_env, &prev_env, &slave, env);
	line = ((t_token *)token->next->content)->token;
	while (head_env != NULL)
	{
		if (ft_strncmp(line, ((t_env *)head_env->content)->key,
				ft_strlen(line) + 1) == 0)
		{
			mod_env(&temp_env, &head_env, &prev_env);
			free_unset(temp_env);
		}
		else
			head_env = head_env->next;
		prev_env = prev_env->next;
	}
	free(slave);
}
