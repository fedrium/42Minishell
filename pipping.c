/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipping.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalee <yalee@student.42.fr.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 15:31:49 by yalee             #+#    #+#             */
/*   Updated: 2023/09/21 22:33:31 by yalee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cp	*make_cp(t_list *head_tokens, int *out, int *in)
{
	t_cp	*cp_head;
	t_cp	*cp_node;
	int		ori_stdout;
	int		ori_stdin;

	cp_head = malloc(sizeof(t_cp));
	cp_head->next = NULL;
	split_args(&cp_head->tokens, &head_tokens);
	cp_node = cp_head;
	while (head_tokens != NULL)
	{
		cp_node->next = malloc(sizeof(t_cp));
		cp_node->next->next = NULL;
		split_args(&cp_node->next->tokens, &head_tokens);
		cp_node = cp_node->next;
	}
	ori_stdout = dup(STDOUT_FILENO);
	ori_stdin = dup(STDIN_FILENO);
	(*out) = ori_stdout;
	(*in) = ori_stdin;
	return (cp_head);
}

t_piping_vars	*init_pipes(t_list *head_tokens)
{
	t_piping_vars	*piping_vars;

	piping_vars = malloc(sizeof(t_piping_vars));
	piping_vars->num_processes = 0;
	piping_vars->child_processes = make_cp(head_tokens,
			&piping_vars->ori_stdout, &piping_vars->ori_stdin);
	piping_vars->cp_head = piping_vars->child_processes;
	piping_vars->pid = 1;
	return (piping_vars);
}

void	start_cp(t_piping_vars **piping_vars, t_list **head_env,
	t_main_vars *main_vars)
{
	if ((*piping_vars)->num_processes)
	{
		dup2((*piping_vars)->child_processes->pipe[0], STDIN_FILENO);
		close((*piping_vars)->child_processes->pipe[0]);
	}
	if ((*piping_vars)->child_processes->next != NULL)
	{
		close((*piping_vars)->child_processes->next->pipe[0]);
		dup2((*piping_vars)->child_processes->next->pipe[1], STDOUT_FILENO);
		close((*piping_vars)->child_processes->next->pipe[1]);
	}
	run_functions_nopp((*piping_vars)->child_processes->tokens,
		head_env, main_vars);
	exit(0);
}

void	organise_args(t_list *head_tokens, t_list **head_env,
	t_main_vars *main_vars)
{
	t_piping_vars	*piping_vars;

	piping_vars = init_pipes(head_tokens);
	if (piping_vars->child_processes->next == NULL)
	{
		run_functions(head_tokens, head_env, main_vars);
		piping_vars->child_processes = piping_vars->child_processes->next;
	}
	while (piping_vars->child_processes != NULL)
	{
		piping_vars->num_processes++;
		if (piping_vars->child_processes->next)
			pipe(piping_vars->child_processes->next->pipe);
		piping_vars->pid = fork();
		if (piping_vars->pid == 0)
			start_cp(&piping_vars, head_env, main_vars);
		else
			iterate_list(&piping_vars);
	}
	while (piping_vars->num_processes > 0)
		wait_cp(piping_vars);
	free_cp(piping_vars->cp_head);
	free(piping_vars);
}

void	run_functions(t_list *head_tokens, t_list **head_env,
	t_main_vars *main_vars)
{
	int	size;

	size = ft_lstsize(head_tokens);
	if (ft_strncmp(((t_token *)head_tokens->content)->token, "echo", 5) == 0)
		echo(head_tokens);
	else if (ft_strncmp(((t_token *)head_tokens->content)->token,
			"unset", 6) == 0)
		unset(head_env, head_tokens, size);
	else if (ft_strncmp(((t_token *)head_tokens->content)->token, "cd", 3) == 0)
		cd(*head_env, head_tokens, size);
	else if (ft_strncmp(((t_token *)head_tokens->content)->token,
			"pwd", 4) == 0)
		pwd();
	else if (ft_strncmp(((t_token *)head_tokens->content)->token,
			"env", 4) == 0)
		pr_env(*head_env);
	else if (ft_strncmp(((t_token *)head_tokens->content)->token,
			"export", 7) == 0)
		export(head_env, head_tokens);
	else if (ft_strncmp(((t_token *)head_tokens->content)->token,
			"exit", 5) == 0)
		exit_func(head_tokens, *head_env, main_vars);
	else
		get_file(head_tokens, *head_env);
}
