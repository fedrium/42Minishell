#include "minishell.h"

int	main(int argc, char **argv, char **env)
{
	char *line;
	t_list	*head_env;
	t_list	*head_tokens;

	head_env = env_init(env);
	while (1)
	{
		line = readline("Minishell$ ");
		head_tokens = tokenize(line);
		if (line && *line)
			add_history(line);
		if (ft_strncmp(line, "echo", 4) == 0)
			echo(line);
		if (ft_strncmp(line, "cd", 2) == 0)
			cd(line);
		if (ft_strncmp(line, "pwd", 3) == 0)
			pwd();
		if (ft_strncmp(line, "env", 3) == 0)
			pr_env(head_env);
		if	(ft_strncmp(line, "export", 6) == 0)
			export(head_env);
		if (ft_strcmp((char *)head_tokens->content, "test") == 0)
		{
			printf("%s\n", (char *)head_tokens->content);
			t_list *node;
			node = head_tokens;
			while (node->next != NULL)
			{
				printf("%s\n", (char *)node->next->content);
				node = node->next;
			}
		}
		free(line);
	}
	(void)argc;
	(void)argv;
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
