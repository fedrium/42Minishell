/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_pwd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalee <yalee@student.42.fr.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 15:19:03 by yalee             #+#    #+#             */
/*   Updated: 2023/09/21 15:41:14 by yalee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pwd(void)
{
	char	*res;

	res = getcwd(NULL, 1024);
	printf("%s\n", res);
	free(res);
}

void	echo(t_list *line)
{
	t_list	*node;

	node = line;
	node = node->next;
	if (node != NULL && ft_strncmp(((t_token *)node->content)->token,
			"-n", 3) == 0)
		return ;
	while (node != NULL)
	{
		printf("%s ", ((t_token *)node->content)->token);
		node = node->next;
	}
	printf("\n");
}
