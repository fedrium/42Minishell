/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_pwd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalee <yalee@student.42.fr.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 15:19:03 by yalee             #+#    #+#             */
/*   Updated: 2023/10/01 23:34:08 by yalee            ###   ########.fr       */
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
	int		flag;
	
	flag = 0;
	node = line;
	node = node->next;
	while (node)
	{
		if (ft_strncmp(((t_token *)node->content)->token,
			"-n", 3) == 0)
			flag = 1;
		else
			break ;
		node = node->next;
	}
	while (node)
	{
		printf("%s", ((t_token *)node->content)->token);
		if (!flag || (flag && node->next))
			printf(" ");
		node = node->next;
	}
	if (!flag)
		printf("\n");
}
