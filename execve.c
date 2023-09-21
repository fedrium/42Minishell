/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalee <yalee@student.42.fr.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 13:34:12 by yalee             #+#    #+#             */
/*   Updated: 2023/09/21 16:00:10 by yalee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_execve_vars	*init_get_file(t_list *head_tokens, t_list *env)
{
	t_execve_vars	*execve_vars;

	execve_vars = malloc(sizeof(t_execve_vars));
	execve_vars->i = 0;
	execve_vars->pid = 1;
	execve_vars->path = path_format(env);
	execve_vars->cmd_arr = convert_list(head_tokens);
	return (execve_vars);
}

int	get_file_abs_path(t_execve_vars *execve_vars, t_list *env)
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
		return (1);
	}
	return (0);
}

void	cheat_norm(t_execve_vars *execve_vars)
{
	execve_vars->cur_dir = opendir(execve_vars->path[execve_vars->i]);
	execve_vars->cur_file = readdir(execve_vars->cur_dir);
}

int	get_file_normal(t_execve_vars *execve_vars, t_list *env)
{
	cheat_norm(execve_vars);
	while (execve_vars->cur_file)
	{
		execve_vars->exec_cmd = strjoin_helper(execve_vars->path
			[execve_vars->i], execve_vars->cmd_arr[0]);
		if (access(execve_vars->exec_cmd, X_OK) == 0)
		{
			execve_vars->pid = fork();
			if (execve_vars->pid == 0)
			{
				execve(strjoin_helper(execve_vars->path[execve_vars->i],
						strjoin_helper("/", execve_vars->cmd_arr[0])),
					execve_vars->cmd_arr, env_arr(env));
				exit(1);
			}
			waitpid(execve_vars->pid, &g_ercode, 0);
			g_ercode = g_ercode % 255;
			free_getfile_norm(execve_vars);
			return (1);
		}
		free(execve_vars->exec_cmd);
		execve_vars->cur_file = readdir(execve_vars->cur_dir);
	}
	closedir(execve_vars->cur_dir);
	return (0);
}

void	get_file(t_list *head_tokens, t_list *env)
{
	t_execve_vars	*execve_vars;

	execve_vars = init_get_file(head_tokens, env);
	while (execve_vars->path[execve_vars->i] != NULL)
	{
		if (execve_vars->cmd_arr[0][0] == '/')
		{
			if (get_file_abs_path(execve_vars, env))
				return ;
		}
		else
		{
			if (get_file_normal(execve_vars, env))
				return ;
		}
		execve_vars->i++;
	}
	printf("command not found\n");
	g_ercode = 127;
	free_2dar(execve_vars->cmd_arr);
	free_2dar(execve_vars->path);
	free(execve_vars);
}
