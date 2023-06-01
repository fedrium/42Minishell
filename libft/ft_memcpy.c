/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyu-xian <cyu-xian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/05 16:01:37 by cyu-xian          #+#    #+#             */
/*   Updated: 2022/05/09 13:49:55 by cyu-xian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t	i;
	char	*j;
	char	*l;

	i = 0;
	j = (char *)dst;
	l = (char *)src;
	if (!dst && !src)
		return (0);
	while (n > i)
	{
		j[i] = l[i];
		i++;
	}
	return (dst);
}
