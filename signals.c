/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalee <yalee@student.42.fr.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 12:39:15 by yalee             #+#    #+#             */
/*   Updated: 2023/09/15 13:12:31 by yalee            ###   ########.fr       */
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

void	sig()
{
	setatt();
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	setatt()
{
	struct termios att;

	tcgetattr(STDIN_FILENO, &att);
    att.c_lflag &= ~ ECHOCTL;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &att);
}