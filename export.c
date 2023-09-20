/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalee <yalee@student.42.fr.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 17:32:10 by yalee             #+#    #+#             */
/*   Updated: 2023/09/20 14:28:36 by yalee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void export(t_list **head_env, t_list *head_tokens)
{
	int printed;
	char *smallest_key;

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
		add_env(head_env, head_tokens);
}

int export_syntax_check(t_list *node)
{
	char *str;
	int i;

	str = ((t_token *)node->content)->token;
	i = 0;
	if (!ft_isalpha(str[0]))
	{
		dprintf(2, "export syntax error!\n");
		return (0);
	}
	while (str[i])
	{
		if (str[i] == ' ')
		{
			dprintf(2, "export syntax error!\n");
			return (0);
		}
		i++;
	}
	return (1);
}

void add_to_envlst(t_list **head_env, t_list *node, t_env *env)
{
	char **splitted_args;
	t_list *node_env;
	t_list *new_env;

	env = malloc(sizeof(t_env));
	node_env = *head_env;
	splitted_args = ft_split(((t_token *)node->content)->token, '=');
	env->key = splitted_args[0];
	env->value = splitted_args[1];
	while (node_env != NULL)
	{
		t_env *temp = (t_env *)node_env->content;
		if (ft_strncmp(temp->key, env->key, ft_strlen(env->key)) == 0)
		{
			free(temp->value);
			temp->value = ft_strdup(env->value);
			free(env->key);
			free(env);
			return;
		}
		node_env = node_env->next;
	}
	new_env = ft_lstnew(env);
	ft_lstadd_back(head_env, new_env);
	free(splitted_args);
}

void add_env(t_list **head_env, t_list *head_tokens)
{
	t_list	*node;
	t_env	*env;

	node = head_tokens;
	if (node->next == NULL)
	{
		dprintf(2, "export error!\n");
		return;
	}
	node = node->next;
	while (node != NULL)
	{
		if (export_syntax_check(node))
			add_to_envlst(head_env, node, env);
		node = node->next;
	}
}

char *find_smallest_key(t_list *head, char *smallest_key)
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

int print_output(t_list *head, char *smallest_key)
{
	int printed;
	int i;
	t_list *node;

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