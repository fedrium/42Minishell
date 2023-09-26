/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_tools.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalee <yalee@student.42.fr.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 15:26:29 by yalee             #+#    #+#             */
/*   Updated: 2023/09/26 16:42:33 by yalee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_tl_str(t_list *node)
{
	return (((t_token *)node->content)->token);
}

char	*lexer_strjoin(char *str1, char *str2)
{
	char	*temp1;
	char	*temp2;
	char	*new;

	temp1 = ft_strdup(str1);
	free(str1);
	temp2 = ft_strdup(str2);
	new = ft_strjoin(temp1, temp2);
	free(temp1);
	free(temp2);
	return (new);
}

void	expansion_get_value(t_list **tenv, char **key, char **value)
{
	while ((*tenv) != NULL)
	{
		if (ft_strncmp((*key), ((t_env *)(*tenv)->content)->key,
			ft_strlen((*key))) == 0)
		{
			(*value) = ft_strdup(((t_env *)(*tenv)->content)->value);
			break ;
		}
		(*tenv) = (*tenv)->next;
	}
}
