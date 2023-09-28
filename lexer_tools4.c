/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_tools4.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyu-xian <cyu-xian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 19:30:44 by cyu-xian          #+#    #+#             */
/*   Updated: 2023/09/28 19:31:39 by cyu-xian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cleanse_vars	*init_cleanse(t_list *node)
{
	t_cleanse_vars	*cleanse_vars;

	cleanse_vars = malloc(sizeof(t_cleanse_vars));
	cleanse_vars->i = 0;
	cleanse_vars->in_dquote = -1;
	cleanse_vars->in_squote = -1;
	cleanse_vars->temp = ((t_token *)node->content)->token;
	cleanse_vars->join[1] = '\0';
	cleanse_vars->new = ft_strdup("");
	return (cleanse_vars);
}
