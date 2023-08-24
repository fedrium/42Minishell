#include "minishell.h"

void	print_2d_arr(char **env)
{
	int	i;

	i = 0;
	while(env[i])
	{
		printf("%s\n", env[i]);
		i++;
	}
	// printf("%s\n", env[i]);
}

char	*getvalue(t_list *env, char *key)
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

char	**env_arr(t_list *env)
{
	int		size;
	int		i;
	char	*str;
	char	**array;

	size = ft_lstsize(env);
	// printf("%lu\n", sizeof(char *) * size + 1);
	array = (char **)malloc(sizeof(char *) * (size + 1));
	array[size] = NULL;
	i = 0;
	while (env != NULL)
	{
		str = ft_strjoin(((t_env *)env->content)->key, "=");
		array[i] = ft_strjoin(str, ((t_env *)env->content)->value);
		free(str);
		// printf("%s\n", array[i]);
		i++;
		env = env->next;
	}
	// print_2d_arr(array);
	return (array);
}

char	**path_format(t_list *env)
{
	char	**array;
	char	*str;

	str = getvalue(env, "PATH");
	// printf("%s\n", str);
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
	// printf("%s, size: %d\n", ((t_token *)node->content)->token, ft_lstsize(node));
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


void	get_file(t_list *head_tokens, t_list *env)
{
	DIR		*cur_dir;
	struct	dirent *cur_file;
	char	**path;
	char	**cmd_arr;
	int		i;

	i = 0;
	path = path_format(env);
	// print_2d_arr(env_arr(env));
	cmd_arr = convert_list(head_tokens);
	while (path[i] != NULL)
	{
		cur_dir = opendir(path[i]);
		if (cur_dir == NULL)
			printf("error");
		while ((cur_file = readdir(cur_dir)))
		{
			if (access(strjoin_helper(path[i], cmd_arr[0]), X_OK) == 0)
			{
				// printf("fd is ok\n");
				// printf("path: %s\n", ft_strjoin(path[i], ft_strjoin("/", cmd_arr[0])));
				// print_2d_arr(cmd_arr);
				// print_2d_arr(env_arr(env));
				execve(ft_strjoin(path[i], ft_strjoin("/", cmd_arr[0])), cmd_arr, env_arr(env));
				// printf("fd is ok\n");
				return;
			}
		}
		i++;
		closedir(cur_dir);
	}
	printf("command not found\n");
	return;
}
