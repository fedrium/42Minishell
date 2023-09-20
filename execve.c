/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalee <yalee@student.42.fr.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 13:34:12 by yalee             #+#    #+#             */
/*   Updated: 2023/09/20 14:03:04 by yalee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

t_execve_vars	*init_get_file(t_list *head_tokens, t_list *env)
{
	t_execve_vars *execve_vars = malloc(sizeof(t_execve_vars));

	execve_vars->i = 0;
	execve_vars->pid = 1;
	execve_vars->path = path_format(env);
	execve_vars->cmd_arr = convert_list(head_tokens);
	return (execve_vars);
}

void	free_getfile_norm(t_execve_vars *execve_vars)
{
	free_2dar(execve_vars->cmd_arr);
	free_2dar(execve_vars->path);
	free(execve_vars->exec_cmd);
	closedir(execve_vars->cur_dir);
}

void	get_file_abs_path(t_execve_vars *execve_vars, t_list *env)
{
	if (access(execve_vars->cmd_arr[0], X_OK) == 0)
	{
		execve_vars->pid = fork();
		if (execve_vars->pid == 0)
		{
			g_ercode = (execve(execve_vars->cmd_arr[0],
				execve_vars->cmd_arr, env_arr(env)) % 225);
			exit(1);
		}
		waitpid(execve_vars->pid, &g_ercode, 0);
		g_ercode = g_ercode % 255;
		free_2dar(execve_vars->cmd_arr);
		free_2dar(execve_vars->path);
		return;
	}
}

void	get_file_normal(t_execve_vars *execve_vars, t_list *env)
{
	execve_vars->cur_dir = opendir(execve_vars->path[execve_vars->i]);
	while ((execve_vars->cur_file = readdir(execve_vars->cur_dir)))
	{
		execve_vars->exec_cmd = strjoin_helper(execve_vars->path[execve_vars->i],
			execve_vars->cmd_arr[0]);
		if (access(execve_vars->exec_cmd, X_OK) == 0)
		{
			execve_vars->pid = fork();
			if (execve_vars->pid == 0)
			{
				execve(strjoin_helper
					(execve_vars->path[execve_vars->i], strjoin_helper
					("/", execve_vars->cmd_arr[0])), execve_vars->cmd_arr,
					env_arr(env));
				exit(1);
			}
			waitpid(execve_vars->pid, &g_ercode, 0);
			g_ercode = g_ercode % 255;
			free_getfile_norm(execve_vars);
			return;
		}
		free(execve_vars->exec_cmd);
	}
	closedir(execve_vars->cur_dir);
}

void get_file(t_list *head_tokens, t_list *env)
{
	t_execve_vars	*execve_vars;
	
	execve_vars = init_get_file(head_tokens, env);
	while (execve_vars->path[execve_vars->i] != NULL)
	{
		if (execve_vars->cmd_arr[0][0] == '/')
			get_file_abs_path(execve_vars, env);
		else
			get_file_normal(execve_vars, env);
		execve_vars->i++;
	}
	printf("command not found\n");
	g_ercode = 127;
	free_2dar(execve_vars->cmd_arr);
	free_2dar(execve_vars->path);
	free(execve_vars);
}

void get_file_abs_path_nopp(t_execve_vars *execve_vars, t_list *env)
{
	if (access(execve_vars->cmd_arr[0], X_OK) == 0)
	{
		g_ercode = (execve(execve_vars->cmd_arr[0],
			execve_vars->cmd_arr, env_arr(env)) % 225);
		waitpid(execve_vars->pid, &g_ercode, 0);
		g_ercode = g_ercode % 255;
		free_2dar(execve_vars->cmd_arr);
		free_2dar(execve_vars->path);
		return;
	}
}

void get_file_normal_nopp(t_execve_vars *execve_vars, t_list *env)
{
	execve_vars->cur_dir = opendir(execve_vars->path[execve_vars->i]);
	while ((execve_vars->cur_file = readdir(execve_vars->cur_dir)))
	{
		execve_vars->exec_cmd = strjoin_helper(execve_vars->path[execve_vars->i],
											   execve_vars->cmd_arr[0]);
		if (access(execve_vars->exec_cmd, X_OK) == 0)
		{
			g_ercode = (execve(strjoin_helper(execve_vars->path[execve_vars->i],
				strjoin_helper("/", execve_vars->cmd_arr[0])),
				execve_vars->cmd_arr, env_arr(env)) %225);
			waitpid(execve_vars->pid, &g_ercode, 0);
			g_ercode = g_ercode % 255;
			free_getfile_norm(execve_vars);
			return;
		}
		free(execve_vars->exec_cmd);
	}
	closedir(execve_vars->cur_dir);
}

void get_file_nopp(t_list *head_tokens, t_list *env)
{
	t_execve_vars *execve_vars;

	execve_vars = init_get_file(head_tokens, env);
	while (execve_vars->path[execve_vars->i] != NULL)
	{
		if (execve_vars->cmd_arr[0][0] == '/')
			get_file_abs_path(execve_vars, env);
		else
			get_file_normal(execve_vars, env);
		execve_vars->i++;
	}
	printf("command not found\n");
	g_ercode = 127;
	free_2dar(execve_vars->cmd_arr);
	free_2dar(execve_vars->path);
	free(execve_vars);
}
