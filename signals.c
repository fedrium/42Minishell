/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalee <yalee@student.42.fr.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 12:39:15 by yalee             #+#    #+#             */
/*   Updated: 2023/09/15 15:18:02 by yalee            ###   ########.fr       */
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

void	exit_signal(int num)
{
	if (num == SIGQUIT)
	{
		write(1, "exit\n", 5);
		exit(0);
	}
}

void	sig(t_list *head_tokens, t_list *head_env)
{
	setatt();
	(void)head_env;
	(void)head_tokens;
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, exit_signal);
}

void	setatt()
{
	struct termios att;

	tcgetattr(STDIN_FILENO, &att);
    att.c_lflag &= ~ ECHOCTL;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &att);
}