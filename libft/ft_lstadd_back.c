/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyu-xian <cyu-xian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/11 11:27:39 by cyu-xian          #+#    #+#             */
/*   Updated: 2022/05/12 15:14:37 by cyu-xian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*lastn;

	if (lst)
	{
		if (*lst)
		{
			lastn = ft_lstlast(*lst);
			lastn->next = new;
		}
		else
			*lst = new;
	}
}
