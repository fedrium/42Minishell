/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyu-xian <cyu-xian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/07 12:28:29 by cyu-xian          #+#    #+#             */
/*   Updated: 2022/05/09 13:57:51 by cyu-xian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t			i;
	unsigned char	*j;
	unsigned char	*k;

	i = 0;
	j = (unsigned char *)s1;
	k = (unsigned char *)s2;
	if (n == 0)
		return (0);
	while (n > i)
	{
		if (j[i] != k[i])
			return (j[i] - k[i]);
		i++;
	}
	return (0);
}
