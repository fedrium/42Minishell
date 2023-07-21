#include "minishell.h"

char	*getvalue(t_list *env, char *key)
{
	t_env *temp;

	while (env->next != NULL)
	{
		temp = (t_env *)env->content;
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
		env = env->next;
	}
	return array;
}

void	exe(t_list *env, t_list *head_tokens)
{
	char	**array;
	int		pid;	

	char *args[2];
	args[0] = "/bin/ls";
	args[1] = NULL;
	array = env_arr(env);
	pid = fork();
	if (pid == 0)
	{
		execve(args[0], args, array);
	}
	waitpid(pid, NULL, 0);
}