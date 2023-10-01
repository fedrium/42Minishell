/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   piping_nopp.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalee <yalee@student.42.fr.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 15:34:32 by yalee             #+#    #+#             */
/*   Updated: 2023/10/01 21:01:20 by yalee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_tokens(t_list *head_tokens)
{
	while (head_tokens != NULL)
	{
		dprintf(2, "token: %s$\n", ((t_token *)head_tokens->content)->token);
		head_tokens = head_tokens->next;
	}
}

void	run_functions_nopp(t_list *head_tokens, t_list **head_env,
	t_main_vars *main_vars)
{
	int	size;
	int	flag;

	size = ft_lstsize(head_tokens);
	flag = redir_check(head_tokens, main_vars);
	if (ft_strncmp(((t_token *)head_tokens->content)->token, "echo", 5) == 0)
		echo(head_tokens);
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
	else if (ft_strncmp(((t_token *)head_tokens->content)->token, "exit",
			5) == 0)
		exit_func(head_tokens, *head_env, main_vars);
	else
	{
		get_file_nopp(head_tokens, *head_env);
	}
	if (flag)
	{
		unlink(".temp");
		dup2(main_vars->out, 1);
		dup2(main_vars->in, 0);
	}
}
