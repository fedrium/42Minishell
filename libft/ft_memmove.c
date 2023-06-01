/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyu-xian <cyu-xian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/06 10:06:48 by cyu-xian          #+#    #+#             */
/*   Updated: 2022/05/09 11:40:21 by cyu-xian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t n)
{
	size_t	i;
	char	*l;
	char	*k;

	i = 0;
	l = (char *)src;
	k = (char *)dst;
	if (src > dst)
	{
		while (n > i)
		{
			k[i] = l[i];
			i++;
		}
	}
	if (dst > src)
	{
		while (n > i)
		{
			k[n - 1] = l[n - 1];
			n--;
		}
	}
	return (k);
}
