#include "minishell.h"

int main(int argc, char **argv, char **env)
{
	char *line;
	t_list *head_env;
	t_list *head_tokens;

	head_env = env_init(env);
	while (1)
	{
		line = readline("Minishell$ ");
		head_tokens = tokenize(line, head_env);
		check_head_tokens(head_tokens, line);
		if (line[0] != '\0' && !check_invalid(head_tokens, 0))
		{
			if (line && *line)
        		add_history(line);
			// organise_args(head_tokens, head_env);
			if (redir_check(head_tokens, head_env) == 1)
				continue;
			// if (here_doc)
			// {
			// 	waitpid(0, NUL, 0);
			// }
			run_functions(head_tokens, head_env);
		}
		free(line);
	}
	(void)argc;
	(void)argv;
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
	node = node->next;
	while (node != NULL)
	{
		printf("%s ", ((t_token *)node->content)->token);
		node = node->next;
	}
	printf("\n");
}

void unset(t_list *env, char *line)
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