#include "minishell.h"

int	main(int argc, char **argv)
{
	char *line;

	while (1)
	{
		line = readline("Minishell$ ");
		if (line && *line)
			add_history(line);
		if (ft_strncmp(line, "echo", 4) == 0)
			echo(line);
		free(line);
	}
	(void)argc;
	(void)argv;
}

void	echo(char *line)
{
	char 	**array;
	int		i;

	array = ft_split(line, ' ');
	i = 1;
	if (ft_strncmp(array[1], "-n", 2) == 0)
		i = 2;
	while (array[i] != 0)
	{
		ft_putstr_fd(array[1], 0);
		i++;
	}
	if (ft_strncmp(array[1], "-n", 2) != 0)
		printf("\n");
}