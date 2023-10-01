/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalee <yalee@student.42.fr.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 17:32:10 by yalee             #+#    #+#             */
/*   Updated: 2023/10/01 23:20:19 by yalee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	export(t_list **head_env, t_list *head_tokens)
{
	int		printed;
	char	*smallest_key;

	if (head_tokens->next == NULL)
	{
		smallest_key = 0;
		printed = 0;
		while (printed < ft_lstsize((*head_env)))
		{
			smallest_key = find_smallest_key((*head_env), smallest_key);
			printed += print_output((*head_env), smallest_key);
		}
	}
	else
		if (export_syntax_check(head_tokens->next))
			add_env(head_env, head_tokens);
}

int	export_syntax_check(t_list *node)
{
	char	*str;
	int		i;
	int		quote;
	int		squote;

	str = ((t_token *)node->content)->token;
	quote = -1;
	squote = -1;
	i = 0;
	if (!ft_isalpha(str[0]))
	{
		printf("export syntax error!\n");
		return (0);
	}
	while (str[i] != '=')
	{
		if (str[i] == ' ')
		{
			printf("export syntax error!\n");
			return (0);
		}
		i++;
	}
	i++;
	while (str[i])
	{
		if (str[i] == '"')
			quote *= -1;
		if (str[i] == 39)
			squote *= -1;
		if (str[i] == ' ' && (quote > 0 || squote > 0))
		{
			printf("export syntax error!\n");
			return (0);
		}
		i++;
	}
	return (1);
}

void	free_temp(t_env *temp, t_env *env)
{
	free(temp->value);
	temp->value = ft_strdup(env->value);
	free(env->key);
	free(env);
}

void	add_to_envlst(t_list **head_env, t_list *node, t_env *env)
{
	char	**splitted_args;
	t_list	*node_env;
	t_list	*new_env;
	t_env	*temp;

	env = malloc(sizeof(t_env));
	node_env = *head_env;
	splitted_args = ft_split(((t_token *)node->content)->token, '=');
	env->key = splitted_args[0];
	env->value = splitted_args[1];
	while (node_env != NULL)
	{
		temp = (t_env *)node_env->content;
		if (ft_strncmp(temp->key, env->key, ft_strlen(env->key)) == 0)
		{
			free_temp(temp, env);
			return ;
		}
		node_env = node_env->next;
	}
	new_env = ft_lstnew(env);
	ft_lstadd_back(head_env, new_env);
	free(splitted_args);
}

void	add_env(t_list **head_env, t_list *head_tokens)
{
	t_list	*node;
	t_env	*env;

	node = head_tokens;
	if (node->next == NULL)
	{
		dprintf(2, "export error!\n");
		return ;
	}
	node = node->next;
	while (node != NULL)
	{
		add_to_envlst(head_env, node, env);
		node = node->next;
	}
}
