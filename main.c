#include "minishell.h"

int	main(int argc, char **argv, char **env)
{
	char *line;
	t_node node;

	
	while (1)
	{
		line = readline("Minishell$ ");
		if (line && *line)
			add_history(line);
		if (ft_strncmp(line, "echo", 4) == 0)
			echo(line);
		if (ft_strncmp(line, "cd", 2) == 0)
			cd(line);
		if (ft_strncmp(line, "pwd", 3) == 0)
			pwd();
		if (ft_strncmp(line, "env", 3) == 0)
			pr_env(env);
		free(line);
	}
	(void)argc;
	(void)argv;
}

t_node	*transfer(t_node node, char **env)
{
	int		i;
	char	**arr;
	t_node	head;
	t_node	*node;

	i = 0;
	node = head;
	while (env[i])
	{
		arr = ft_split(env[i], '=');
		node->
	}
}

void	pr_env(char **env)
{
	int	i;

	i = 0;
	while (env[i] != 0)
	{
		printf("%s\n", env[i]);
		i++;
	}
}

void	pwd(void)
{
	char	*res;

	res = getcwd(NULL, 1024);
	printf("%s\n", res);
	free(res);
}

void	cd(char *line)
{
	char	**array;

	array = ft_split(line, ' ');
	if (array[2] != 0)
	{
		printf("Too many arguments\n");
		return;
	}
	chdir(array[1]);
}

void	echo(char *line)
{
	char 	**array;
	int		i;

	array = ft_split(line, ' ');
	i = 1;
	if (array[1] != 0 && ft_strncmp(array[1], "-n", 2) == 0)
		i = 2;
	while (array[i] != 0)
	{
		ft_putstr_fd(array[i], 0);
		i++;
	}
	if (array[1] != 0 && ft_strncmp(array[1], "-n", 2) != 0)
		printf("\n");
}