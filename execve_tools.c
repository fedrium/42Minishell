/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve_tools.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyu-xian <cyu-xian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 15:21:52 by yalee             #+#    #+#             */
/*   Updated: 2023/09/28 19:39:46 by cyu-xian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

char	*getvalue(t_list *env, char *key)
{
	t_env	*temp;

	while (env)
	{
		temp = (t_env *)env->content;
		if (ft_strncmp(temp->key, key, ft_strlen(key)) == 0)
			return (temp->value);
		else
			env = env->next;
	}
	return (NULL);
}

void	free_getfile_norm(t_execve_vars *execve_vars)
{
	free_2dar(execve_vars->cmd_arr);
	free_2dar(execve_vars->path);
	free(execve_vars->exec_cmd);
	closedir(execve_vars->cur_dir);
	free(execve_vars);
}

char	**env_arr(t_list *env)
{
	int		size;
	int		i;
	char	*str;
	char	**array;

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

char	**path_format(t_list *env)
{
	char	**array;
	char	*str;

	str = getvalue(env, "PATH");
	if (str == NULL)
		return (NULL);
	array = ft_split(str, ':');
	return (array);
}
