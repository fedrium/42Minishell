/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve_nopp.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalee <yalee@student.42.fr.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 15:19:59 by yalee             #+#    #+#             */
/*   Updated: 2023/09/28 22:11:25 by yalee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_file_abs_path_nopp(t_execve_vars *execve_vars, t_list *env)
{
	if (access(execve_vars->cmd_arr[0], X_OK) == 0)
	{
		g_ercode = (execve(execve_vars->cmd_arr[0],
					execve_vars->cmd_arr, env_arr(env)) % 225);
		waitpid(execve_vars->pid, &g_ercode, 0);
		g_ercode = g_ercode % 255;
		free_2dar(execve_vars->cmd_arr);
		free_2dar(execve_vars->path);
		return (1);
	}
	return (0);
}

int	get_file_normal_nopp(t_execve_vars *execve_vars, t_list *env)
{
	execve_vars->cur_dir = opendir(execve_vars->path[execve_vars->i]);
	execve_vars->cur_file = readdir(execve_vars->cur_dir);
	while (execve_vars->cur_file)
	{
		execve_vars->exec_cmd = strjoin_helper(
				execve_vars->path[execve_vars->i], execve_vars->cmd_arr[0]);
		if (access(execve_vars->exec_cmd, X_OK) == 0)
		{
			g_ercode = (execve(strjoin_helper(execve_vars->path[execve_vars->i],
							strjoin_helper("/", execve_vars->cmd_arr[0])),
						execve_vars->cmd_arr, env_arr(env)) % 225);
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

void	get_file_nopp(t_list *head_tokens, t_list *env)
{
	t_execve_vars	*execve_vars;

	execve_vars = init_get_file(head_tokens, env);
	if (execve_vars == NULL)
		return ;
	while (execve_vars->path[execve_vars->i] != NULL)
	{
		if (execve_vars->cmd_arr[0][0] == '/')
		{
			if (get_file_abs_path_nopp(execve_vars, env))
				return ;
		}
		else
		{
			if (get_file_normal_nopp(execve_vars, env))
				return ;
		}
		execve_vars->i++;
	}
	printf("command not found\n");
	g_ercode = 127;
	free_execve(execve_vars);
}

char	**convert_list(t_list *head_tokens)
{
	t_list	*node;
	char	**cmd_arr;
	int		i;

	node = head_tokens;
	i = 0;
	if (head_tokens == NULL)
		return (NULL);
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

void	free_execve(t_execve_vars *execve_vars)
{
	if (execve_vars->cmd_arr)
		free_2dar(execve_vars->cmd_arr);
	if (execve_vars->path)
		free_2dar(execve_vars->path);
	free(execve_vars);
}
