/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyu-xian <cyu-xian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/06 15:24:30 by cyu-xian          #+#    #+#             */
/*   Updated: 2022/05/09 12:06:41 by cyu-xian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int	i;
	int	j;

	i = 0;
	j = ft_strlen((char *)s);
	if (c == 0)
		return ((char *)&s[j]);
	if (c > 256)
		c = c - 256;
	while (j >= i)
	{
		if (s[j] == (char)c)
		{
			return ((char *)&s[j]);
		}
		j--;
	}
	return (0);
}
