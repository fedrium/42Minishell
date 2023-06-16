#include "minishell.h"

int	main(int argc, char **argv, char **env)
{
	char *line;
	t_list	*head;

	head = env_init(env);
	while (1)
	{
		line = readline("Minishell$ ");
		if (line && *line)
			add_history(line);
		if (ft_strncmp(line, "echo", 4) == 0)
			echo(line);
		if (ft_strncmp(line, "cd", 2) == 0)
			cd(line, head);
		if (ft_strncmp(line, "pwd", 3) == 0)
			pwd();
		if (ft_strncmp(line, "env", 3) == 0)
			pr_env(head);
		if	(ft_strncmp(line, "export", 6) == 0)
			export(head);
		if	(ft_strncmp(line, "unset", 5) == 0)
			unset(line, head);
		free(line);
	}
	(void)argc;
	(void)argv;
}

void	pr_env(t_list	*head)
{
	while (head->next != NULL)
	{
		t_env *temp = (t_env *)head->content;
		if (temp->key != NULL || temp->value != NULL)
			printf("%s=%s\n", temp->key, temp->value);
		head = head->next;
	}
}

void	pwd(void)
{
	char	*res;

	res = getcwd(NULL, 1024);
	printf("%s\n", res);
	free(res);
}

void	cd(char *line, t_list *head)
{
	char	**array;
	char	*pwd;
	char	*oldpwd;

	oldpwd = getcwd(NULL, 1024);
	array = ft_split(line, ' ');
	if (array[2] != 0)
	{
		printf("Too many arguments\n");
		return;
	}
	chdir(array[1]);
	pwd = getcwd(NULL, 1024);
	while (head->next != NULL)
	{
		t_env *temp = (t_env *)head->content;
		if (ft_strncmp(temp->key, "PWD", 3) == 0)
		{
			temp->value = pwd;
		}
		if (ft_strncmp(temp->key, "OLDPWD", 6) == 0)
		{
			temp->value = oldpwd;
		}
		head = head->next;
	}
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
