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
		if (line[0] != '\0' && is_valid_lst(head_tokens, head_env))
		{
			if (line && *line)
				add_history(line);
			if (ft_strncmp(line, "echo", 4) == 0)
				echo(head_tokens);
			if (ft_strncmp(line, "cd", 2) == 0)
				cd(head_env, ((t_token *)head_tokens->next->content)->token);
			if (ft_strncmp(line, "pwd", 3) == 0)
				pwd();
			if (ft_strncmp(line, "env", 3) == 0)
				pr_env(head_env);
			if (ft_strncmp(line, "unset", 5) == 0)
				unset(head_env, ((t_token *)head_tokens->next->content)->token);
			// if	(ft_strncmp(line, "export", 6) == 0)
			// 	export(head_env, head_tokens);
			if (ft_strncmp(((t_token *)head_tokens->content)->token, "export", 6) == 0)
				export(head_env, head_tokens);
			if (ft_strncmp(((t_token *)head_tokens->content)->token, "test", 4) == 0)
			{
				t_list *node;
				node = head_tokens;
				printf("%s\n", ((t_token *)node->content)->token);
				while (node->next != NULL)
				{
					printf("%s\n", ((t_token *)node->next->content)->token);
					if (node->next != NULL)
						node = node->next;
				}
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

void	cd(t_list *env, char *line)
{
	t_env	*temp;
	char	*res;

	printf("line: %s\n", line);	
	chdir(line);
	res = getcwd(NULL, 1024);
	while (env->next != NULL)
	{
		temp = (t_env *)env->content;
		if (ft_strncmp("PWD", temp->key, 3) == 0)
			temp->value = res;
		env = env->next;
	}
}

void	echo(t_list *line)
{
	t_list	*node;

	node = line;
	while (node->next != NULL)
	{
		printf("%s ", ((t_token *)node->next->content)->token);
		if (node->next != NULL)
			node = node->next;
	}
	printf("\n");
}

void	unset(t_list *env, char *line)
{
	t_env *temp;

	while (env->next != NULL)
	{
		temp = (t_env *)env->content;
		if (ft_strncmp(line, temp->key, 3) == 0)
		{
			free(temp->key);
			free(temp->value);
		}
		env = env->next;
	}
}