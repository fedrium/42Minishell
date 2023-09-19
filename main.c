/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalee <yalee@student.42.fr.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 00:37:49 by yalee             #+#    #+#             */
/*   Updated: 2023/09/20 00:40:07 by yalee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_main_vars	*init_basic(char **env)
{
	t_main_vars	*main_vars;

	main_vars = malloc(sizeof(t_main_vars));
	main_vars->in = dup(1);
	main_vars->out = dup(0);
	main_vars->head_env = env_init(env);
	g_ercode = 0;
	tcgetattr(STDIN_FILENO, &main_vars->saved);
	return (main_vars);
}

void	signulll_exit(t_main_vars *main_vars)
{
	printf("Minishell$ exit\n");
	exit_func(main_vars->head_tokens, main_vars->head_env, main_vars);
}

void	executor(t_main_vars *main_vars)
{
	check_head_tokens(main_vars->head_tokens, main_vars->line);
	if (main_vars->line[0] != '\0' && !check_invalid(main_vars->head_tokens, 0))
	{
		if (main_vars->line && (*main_vars->line))
			add_history(main_vars->line);
		redir_check(main_vars->head_tokens);
		organise_args(main_vars->head_tokens, &(main_vars->head_env),
			main_vars);
		unlink(".temp");
		dup2(main_vars->out, 1);
		dup2(main_vars->in, 0);
	}
}

int	main(int argc, char **argv, char **env)
{
	t_main_vars	*main_vars;

	main_vars = init_basic(env);
	while (1)
	{
		sig(main_vars->head_tokens, main_vars->head_env);
		main_vars->line = readline("Minishell$ ");
		if (main_vars->line == NULL)
			signulll_exit(main_vars);
		main_vars->head_tokens = tokenize(main_vars->line, main_vars->head_env);
		executor(main_vars);
		free(main_vars->line);
	}
}

void pwd(void)
{
	char *res;

	res = getcwd(NULL, 1024);
	printf("%s\n", res);
	free(res);
}

char *get_env(t_list *env, char *line)
{
	t_env *temp;

	while (env->next != NULL)
	{
		temp = (t_env *)env->content;
		if (ft_strncmp(temp->key, line, ft_strlen(line) + 1) == 0)
			return (temp->value);
		env = env->next;
	}
	return (0);
}

int	cd_stay_on_current_dir(t_list *env)
{
	chdir(get_env(env, "HOME"));
	return (1);
}

void	cd_update_env(t_list *env, char *old, char *now)
{
	t_env *temp;

	while (env != NULL)
	{
		temp = (t_env *)env->content;
		if (ft_strncmp("PWD", temp->key, 3) == 0)
		{
			free(temp->value);
			temp->value = ft_strdup(now);
		}
		if (ft_strncmp("OLDPWD", temp->key, 6) == 0)
		{
			free(temp->value);
			temp->value = ft_strdup(old);
		}
		env = env->next;
	}
}

void cd(t_list *env, t_list *token, int size)
{
	char *now;
	char *line;
	char *old;

	if (size == 1 && cd_stay_on_current_dir(env))
		return;
	line = ((t_token *)token->next->content)->token;
	old = getcwd(NULL, 1024);
	if (line[0] == '~')
		chdir(get_env(env, "HOME"));
	else if (chdir(line) == -1)
		printf("bash: cd: %s: No such file or directory\n", line);
	now = getcwd(NULL, 1024);
	cd_update_env(env, old, now);
	free(old);
	free(now);
}


void echo(t_list *line)
{
	t_list *node;

	node = line;
	node = node->next;
	if (node != NULL && ft_strncmp(((t_token *)node->content)->token, "-n", 3) == 0)
		return ;
	while (node != NULL)
	{
		printf("%s ", ((t_token *)node->content)->token);
		node = node->next;
	}
	printf("\n");
}

void	free_unset(t_list *temp_env)
{
	free(((t_env *)temp_env->content)->key);
	free(((t_env *)temp_env->content)->value);
	free(temp_env->content);
	free(temp_env);
}

void	init_unset(t_list **head_env, t_list **prev_env, t_list **slave, t_list **env)
{
	*head_env = *env;
	*prev_env = ft_lstnew(NULL);
	*slave = *prev_env;
	(*prev_env)->next = *head_env;
}

void unset(t_list **env, t_list *token, int size)
{
	t_list	*temp_env;
	t_list	*prev_env;
	t_list	*head_env;
	t_list	*slave;
	char	*line;

	if (size == 1)
		return;
	init_unset(&head_env, &prev_env, &slave, env);
	line = ((t_token *)token->next->content)->token;
	while (head_env != NULL)
	{
		if (ft_strncmp(line, ((t_env *)head_env->content)->key, ft_strlen(line) + 1) == 0)
		{
			temp_env = head_env;
			head_env = head_env->next;
			if (prev_env->content != NULL)
				prev_env->next = head_env;
			free_unset(temp_env);
		}
		else
			head_env = head_env->next;
		prev_env = prev_env->next;
	}
	free(slave);
}

void    exit_func(t_list *head_tokens, t_list *head_env, t_main_vars *main_vars)
{
	lst_free_env(head_env);
	free(main_vars);
    system("leaks minishell");
    exit(0);
}