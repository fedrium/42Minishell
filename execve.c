#include "minishell.h"

char	*getvalue(t_list *env, char *key)
{
	t_env *temp;

	while (env)
	{
		temp = (t_env *)env->content;
		printf("strlen: %s\n", temp->key);
		if (ft_strncmp(temp->key, key, ft_strlen(key)) == 0)
			return (temp->value);
		else
			env = env->next;
	}
	return NULL;
}

char	**env_arr(t_list *env)
{
	int		size;
	int		i;
	char	*str;
	char	**array;
	t_env	*temp;

	size = ft_lstsize(env);
	array = malloc(sizeof(char *) * size);
	i = 0;
	while (env->next != NULL)
	{
		temp = (t_env *)env->content;
		str = ft_strjoin(temp->key, "=");
		array[i] = ft_strjoin(str, temp->value);
		free(str);
		env = env->next;
	}
	return (array);
}

char	**path_format(t_list *env)
{
	char	**array;
	char	*str;

	str = getvalue(env, "PATH");
	array = ft_split(str, ':');
	return (array);
}

//make a function for running through dir

char	*strjoin_helper(char *path, char *cmd)
{
	char	*res;
	char	*temp1;
	char	*temp2;

	res = ft_strjoin(path, "/");
	temp1 = ft_strdup(res);
	free(res);
	res = ft_strjoin(temp1, cmd);
	free(temp1);
	temp2 = ft_strdup(res);
	free(res);
	return (temp2);
}

char	**convert_list(t_list *head_tokens)
{
	t_list	*node;
	char	**cmd_arr;
	int		i;

	node = head_tokens;
	i = 0;
	if (head_tokens == NULL)
		return NULL;
	cmd_arr = malloc(sizeof(char *) * ft_lstsize(head_tokens));
	while (head_tokens != NULL)
	{
		cmd_arr[i] = ft_strdup(((t_token *)node->content)->token);
		i++;
	}
	return (cmd_arr);
}

int		get_file(t_list *head_tokens, t_list *env)
{
	DIR		*cur_dir;
	struct	dirent *cur_file;
	char	**path;
	char	**cmd_arr;
	int		i;

	i = 0;
	path = path_format(env);
	//change llist into 2d arr
	cmd_arr = convert_list(head_tokens);
	while (path[i] != NULL)
	{
		cur_dir = opendir(path[i]);
		if (cur_dir == NULL)
			printf("error");
		while ((cur_file = readdir(cur_dir)))
		{
			// if (get_file_helper(path[i], cmd_arr[0], cmd, env) == 0)
			// {
			// 	freeing(cmd_arr);
			// 	freeing(path);
			// 	return (0);
			// }
			if (access(strjoin_helper(path[i], cmd_arr[0]), X_OK) == 0)
			{
				// exe(env, cmd, strjoin_helper(path[i], cmd_arr[0]));
				return (0);
			}
		}
		i++;
		closedir(cur_dir);
	}
	freeing(cmd_arr);
	freeing(path);
	return (1);
}

#//jp = joined path
int	get_file_helper(char *path, char *cmd_arr, char *cmd, t_list *env)
{
	char	*jp;

	jp = strjoin_helper(path, cmd_arr);
	if (access(jp, X_OK) == 0)
	{
		exe(env, cmd, strjoin_helper(path, cmd_arr));
		free(jp);
		return (0);
	}
	free(jp);
	return (1);
}

void	exe(t_list *env, char *cmd, char *path)
{
	char	**array;
	char	**cmd_arr;
	int		pid;

	array = env_arr(env);
	cmd_arr = ft_split(cmd, ' ');
	pid = fork();
	if (pid == 0)
		execve(path, cmd_arr, array);
	freeing(array);
	freeing(cmd_arr);
	waitpid(pid, NULL, 0);
}


void	bin_functions(t_list *head_tokens, t_list *head_env)
{
	char	**path_arr;
	char	**env_arr;
	char	**arguments;

	path_arr = get_paths(head_env);

}