#include "minishell.h"

int	main(int argc, char **argv)
{
	char *line;

	while (line != 0)
	{
		line = readline("Minishell$ ");
		if (line && *line)
			add_history(line);
	}
	(void)argc;
	(void)argv;
}