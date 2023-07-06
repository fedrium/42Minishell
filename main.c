#include "minishell.h"

int main(int argc, char **argv, char **env)
{
	char *line;
	int size;
	t_list *head_env;
	t_list *head_tokens;

	head_env = env_init(env);
	while (1)
	{
		line = readline("Minishell$ ");
		head_tokens = tokenize(line);
		size = ft_lstsize(head_tokens);
		if (line[0] != '\0' && is_valid_lst(head_tokens, head_env))
		{
			if (line && *line)
				add_history(line);
			if (ft_strncmp(((t_token *)head_tokens->content)->token, "echo", 4) == 0)
				echo(head_tokens);
			if (ft_strncmp(((t_token *)head_tokens->content)->token, "cd", 2) == 0)
				cd(head_env, head_tokens, size);
			if (ft_strncmp(((t_token *)head_tokens->content)->token, "pwd", 3) == 0)
				pwd();
			if (ft_strncmp(((t_token *)head_tokens->content)->token, "env", 3) == 0)
				pr_env(head_env);
			if (ft_strncmp(((t_token *)head_tokens->content)->token, "exit", 4) == 0)
				exiting(head_tokens, size);
			if (ft_strncmp(line, "unset", 5) == 0)
				unset(&head_env, head_tokens);
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

void	exiting(t_list *token, int size)
{
	char 	*line;
	int		code;
	int		i;

	i = 0;
	if (size <= 2)
	{
		printf("exit\n");
		exit (0);
	}
	line = ((t_token *)token->next->content)->token;
	while (line[i] != '\0')
	{
		if (ft_isdigit(line[i]) == 0)
		{
			printf("exit\n");
			printf("bash: exit: %s: numeric argument required\n", line);
		}
		i++;
	}
	code = ft_atoi(line);
	printf("exit\n");
	exit (code);
}

void pwd(void)
{
	char *res;

	res = getcwd(NULL, 1024);
	printf("%s\n", res);
	free(res);
}

char *get_env(t_list *env, char *line)
{
	t_env *temp;

	while (env->next != NULL)
	{
		temp = (t_env *)env->content;
		if (ft_strncmp(temp->key, line, ft_strlen(line) + 1) == 0)
			return (temp->value);
		env = env->next;
	}
	return (0);
}

void cd(t_list *env, t_list *token, int size)
{
	t_env *temp;
	char *now;
	char *line;
	char *old;

	printf("size: %i\n", size);
	if (size == 1)
	{
		chdir(get_env(env, "HOME"));
		return;
	}
	line = ((t_token *)token->next->content)->token;
	printf("line: %s\n", line);
	old = getcwd(NULL, 1024);
	if (line[0] == '~')
		chdir(get_env(env, "HOME"));
	else if (chdir(line) == -1)
		printf("bash: cd: %s: No such file or directory\n", line);
	now = getcwd(NULL, 1024);
	while (env->next != NULL)
	{
		temp = (t_env *)env->content;
		if (ft_strncmp("PWD", temp->key, 3) == 0)
			temp->value = now;
		if (ft_strncmp("OLDPWD", temp->key, 6) == 0)
			temp->value = old;
		env = env->next;
	}
}

void echo(t_list *line)
{
	t_list *node;

	node = line;
	while (node->next != NULL)
	{
		printf("%s ", ((t_token *)node->next->content)->token);
		if (node->next != NULL)
			node = node->next;
	}
	printf("\n");
}

void unset(t_list **env, t_list *token)
{
	t_env	*temp;
	t_list	*envt;
	t_list	*head;
	char	*line;

	envt = NULL;
	line = ((t_token *)token->next->content)->token;
	head = *env;
	while (head->next != NULL)
	{
		temp = (t_env *)head->content;
		if (ft_strncmp(line, temp->key, ft_strlen(line)) == 0)
		{
			free(temp->key);
			free(temp->value);
			if (envt == NULL)
			{
				*env = head->next;
				return;
			}
			envt->next = head->next;
		}
		envt = head;
		head = head->next;
	}
}

// void	tlistmover(t_list **env)
// {
// 	t_list	*cur;

// 	cur = *env;
// 	cur = cur->next;
// }