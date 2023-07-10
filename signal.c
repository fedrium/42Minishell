#include "minishell.h"

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
}

void	setatt()
{
	struct termios attributes;

	tcgetattr(STDIN_FILENO, &saved);

	tcgetattr(STDIN_FILENO, &attributes);
    attributes.c_lflag &= ~ ECHOK;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &attributes);
}