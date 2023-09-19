#include "minishell.h"

void print_2d_arr(char **env)
{
	int i;

	i = 0;
	while (env[i])
	{
		printf("%s\n", env[i]);
		i++;
	}
}

char *getvalue(t_list *env, char *key)
{
	t_env *temp;

	while (env)
	{
		temp = (t_env *)env->content;
		// printf("strlen: %s\n", temp->key);
		if (ft_strncmp(temp->key, key, ft_strlen(key)) == 0)
			return (temp->value);
		else
			env = env->next;
	}
	return NULL;
}

char **env_arr(t_list *env)
{
	int size;
	int i;
	char *str;
	char **array;

	size = ft_lstsize(env);
	array = (char **)malloc(sizeof(char *) * (size + 1));
	array[size] = NULL;
	i = 0;
	while (env != NULL)
	{
		str = ft_strjoin(((t_env *)env->content)->key, "=");
		array[i] = ft_strjoin(str, ((t_env *)env->content)->value);
		free(str);
		i++;
		env = env->next;
	}
	return (array);
}

char **path_format(t_list *env)
{
	char **array;
	char *str;

	str = getvalue(env, "PATH");
	array = ft_split(str, ':');
	// if (str != NULL)
	// 	free(str);
	return (array);
}

char *strjoin_helper(char *path, char *cmd)
{
	char *res;
	char *temp1;
	char *temp2;

	res = ft_strjoin(path, "/");
	temp1 = ft_strdup(res);
	free(res);
	res = ft_strjoin(temp1, cmd);
	free(temp1);
	temp2 = ft_strdup(res);
	free(res);
	return (temp2);
}

char **convert_list(t_list *head_tokens)
{
	t_list *node;
	char **cmd_arr;
	int i;

	node = head_tokens;
	i = 0;
	if (head_tokens == NULL)
		return NULL;
	cmd_arr = malloc(sizeof(char *) * (ft_lstsize(node) + 1));
	cmd_arr[ft_lstsize(node)] = NULL;
	while (node != NULL)
	{
		cmd_arr[i] = ft_strdup(((t_token *)node->content)->token);
		i++;
		node = node->next;
	}
	return (cmd_arr);
}

void get_file(t_list *head_tokens, t_list *env)
{
	DIR *cur_dir;
	struct dirent *cur_file;
	char **path;
	char **cmd_arr;
	char *exec_cmd;
	int i;
	pid_t pid;

	i = 0;
	pid = 1;
	path = path_format(env);
	cmd_arr = convert_list(head_tokens);
	while (path[i] != NULL)
	{
		if (cmd_arr[0][0] == '/') // Check if it's an absolute path
		{
			if (access(cmd_arr[0], X_OK) == 0)
			{
				pid = fork();
				if (pid == 0)
				{
					g_ercode = (execve(cmd_arr[0], cmd_arr, env_arr(env)) % 225);
					exit(1); // Exit with a non-zero status to indicate error
				}
				waitpid(pid, &g_ercode, 0);
				g_ercode = g_ercode % 255;
				free_2dar(cmd_arr);
				free_2dar(path);
				return;
			}
		}
		else
		{
			cur_dir = opendir(path[i]);
			if (cur_dir == NULL)
			{
				printf("error");
			}
			while ((cur_file = readdir(cur_dir)))
			{
				exec_cmd = strjoin_helper(path[i], cmd_arr[0]);
				if (access(exec_cmd, X_OK) == 0)
				{
					pid = fork();
					if (pid == 0)
					{
						g_ercode = (execve(strjoin_helper(path[i], strjoin_helper("/", cmd_arr[0])), cmd_arr, env_arr(env)) % 225);
						exit(1); // Exit with a non-zero status to indicate error
					}
					waitpid(pid, &g_ercode, 0);
					g_ercode = g_ercode % 255;
					free_2dar(cmd_arr);
					free_2dar(path);
					free(exec_cmd);
					closedir(cur_dir);
					return;
				}
				free(exec_cmd);
			}
			closedir(cur_dir);
		}
		i++;
	}
	printf("command not found\n");
	g_ercode = 127;
	free_2dar(cmd_arr);
	free_2dar(path);
	free(exec_cmd);
}

void get_file_nopp(t_list *head_tokens, t_list *env)
{
	DIR *cur_dir;
	struct dirent *cur_file;
	char **path;
	char **cmd_arr;
	int i;

	i = 0;
	path = path_format(env);
	cmd_arr = convert_list(head_tokens);
	while (path[i] != NULL)
	{
		if (cmd_arr[0][0] == '/') // Check if it's an absolute path
		{
			if (access(cmd_arr[0], X_OK) == 0)
			{
				g_ercode = (execve(cmd_arr[0], cmd_arr, env_arr(env)) % 225);
				exit(1); // Exit with a non-zero status to indicate error
			}
		}
		else
		{
			cur_dir = opendir(path[i]);
			if (cur_dir == NULL)
			{
				printf("error");
			}
			while ((cur_file = readdir(cur_dir)))
			{
				if (access(strjoin_helper(path[i], cmd_arr[0]), X_OK) == 0)
				{
					g_ercode = (execve(ft_strjoin(path[i], ft_strjoin("/", cmd_arr[0])), cmd_arr, env_arr(env)) % 225);
					exit(1); // Exit with a non-zero status to indicate error
				}
			}
			closedir(cur_dir);
		}
		i++;
	}
	printf("command not found\n");
	g_ercode = 127;
}
