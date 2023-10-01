/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   piping_nopp.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalee <yalee@student.42.fr.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 15:34:32 by yalee             #+#    #+#             */
/*   Updated: 2023/10/02 01:26:13 by yalee            ###   ########.fr       */
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

void	cheese_norm(t_main_vars *main_vars)
{
	unlink(".temp");
	dup2(main_vars->out, 1);
	dup2(main_vars->in, 0);
}

void	run_functions_nopp(t_list *h, t_list **e,
	t_main_vars *main_vars)
{
	int	size;
	int	flag;

	size = ft_lstsize(h);
	flag = redir_check(h, main_vars);
	if (ft_strncmp(((t_token *)h->content)->token, "echo", 5) == 0)
		echo(h);
	else if (ft_strncmp(((t_token *)h->content)->token, "unset", 6) == 0)
		unset(e, h, size);
	else if (ft_strncmp(((t_token *)h->content)->token, "cd", 3) == 0)
		cd(*e, h, size);
	else if (ft_strncmp(((t_token *)h->content)->token, "pwd", 4) == 0)
		pwd();
	else if (ft_strncmp(((t_token *)h->content)->token, "env", 4) == 0)
		pr_env(*e);
	else if (ft_strncmp(((t_token *)h->content)->token,
			"export", 7) == 0)
		export(e, h);
	else if (ft_strncmp(((t_token *)h->content)->token,
			"exit", 5) == 0)
		exit_func(h, *e, main_vars);
	else
		get_file(h, *e);
	if (flag)
		cheese_norm(main_vars);
}
