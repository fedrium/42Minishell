/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalee <yalee@student.42.fr.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 12:39:15 by yalee             #+#    #+#             */
/*   Updated: 2023/09/28 16:06:35 by yalee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>

void	signal_handler(int num)
{
	if (num == SIGINT)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	sig(t_list *head_tokens, t_list *head_env)
{
	setatt();
	(void)head_env;
	(void)head_tokens;
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	cntl_c(int num)
{
	(void)num;
	write(1, "\0", 1);
}

void	setatt(void)
{
	struct termios	att;

	tcgetattr(STDIN_FILENO, &att);
	att.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &att);
}

void	signulll_exit(t_main_vars *main_vars)
{
	printf("Minishell$ exit\n");
	exit_func(main_vars->head_tokens, main_vars->head_env, main_vars);
}
