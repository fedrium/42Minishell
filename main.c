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
			cd(line);
		if (ft_strncmp(line, "pwd", 3) == 0)
			pwd();
		if (ft_strncmp(line, "env", 3) == 0)
			pr_env(head);
		free(line);
	}
	(void)argc;
	(void)argv;
}

t_env	*new_env(char	**splitted_env)
{
	t_env	*env;

	env = malloc(sizeof(t_env));
	env->key = ft_strdup(splitted_env[0]);
	env->value = ft_strdup(splitted_env[1]);
	return (env);
}

t_list	*env_init(char	**env)
{
	t_list	*head;
	t_list	*node;
	int		i;
	char	**splitted_env;

	i = 1;
	splitted_env = ft_split(env[0], '=');
	node = ft_lstnew((void *)new_env(splitted_env));
	head = node;
	while (env[i])
	{
		splitted_env = ft_split(env[i], '=');
		node->next = ft_lstnew((void *)new_env(splitted_env));
		node = node->next;
		i++;
	}
	return (head);
}

void	pr_env(t_list	*head)
{
	while (head->next != NULL)
	{
		t_env *temp = (t_env *)head->content;
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

void	export(t_list	*head)
{
	t_list	*node;
	t_env	temp_env;
	int		printed;
	int		cmp_num;

	node = head;
	printed = 0;
	while (printed < ft_lstsize(head))
	{
		node = head;
		while (node->next != NULL)
		{
			
		}
	}
}
