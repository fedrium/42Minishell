/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyu-xian <cyu-xian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/11 11:27:47 by cyu-xian          #+#    #+#             */
/*   Updated: 2022/05/12 15:14:51 by cyu-xian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*new;
	t_list	*tmp;

	new = *lst;
	while (new)
	{
		tmp = new;
		new = new->next;
		del(tmp->content);
		free(tmp);
	}
	*lst = new;
}
