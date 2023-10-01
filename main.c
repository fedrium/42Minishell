/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalee <yalee@student.42.fr.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 00:37:49 by yalee             #+#    #+#             */
/*   Updated: 2023/10/01 17:40:48 by yalee            ###   ########.fr       */
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

int	main(int argc, char **argv, char **env)
{
	t_main_vars	*main_vars;
	int			flag;

	main_vars = init_basic(env);
	flag = 0;
	while (1)
	{
		if (flag == 1)
		{
			flag = 0;
			continue;
		}
		sig(main_vars->head_tokens, main_vars->head_env);
		main_vars->line = readline("Minishell$ ");
		if (main_vars->line == NULL)
			signulll_exit(main_vars);
		main_vars->head_tokens = tokenize(main_vars->line, main_vars->head_env);
		executor(main_vars, &flag);
		free(main_vars->line);
	}
}

void	executor(t_main_vars *main_vars, int *flag)
{
	check_head_tokens(main_vars->head_tokens, main_vars->line);
	if (main_vars->line[0] != '\0' && !check_invalid(main_vars->head_tokens, 0))
	{
		if (main_vars->line && (*main_vars->line))
			add_history(main_vars->line);
		if (redir_check(main_vars->head_tokens))
		{
			(*flag) = 1;
			organise_args(main_vars->head_tokens, &(main_vars->head_env),
				main_vars);
			unlink(".temp");
			dup2(main_vars->out, 1);
			dup2(main_vars->in, 0);
		}
		else
		{
			dup2(main_vars->out, 1);
			dup2(main_vars->in, 0);
			organise_args(main_vars->head_tokens, &(main_vars->head_env),
				main_vars);
			(*flag) = 0;
		}
	}
}

void	exit_func(t_list *head_tokens, t_list *head_env, t_main_vars *main_vars)
{
	lst_free_env(head_env);
	free(main_vars);
	if (main_vars->line)
		free(main_vars->line);
	system("leaks minishell");
	exit(0);
}
