/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyu-xian <cyu-xian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/06 16:38:18 by cyu-xian          #+#    #+#             */
/*   Updated: 2022/05/09 12:05:55 by cyu-xian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	if (needle[i] == '\0')
		return ((char *)haystack);
	while (len > i && haystack[i] != '\0')
	{
		j = 0;
		while (len > (i + j) && haystack[i + j] == needle [j])
		{
			if (needle[j + 1] == '\0')
				return ((char *)&haystack[i]);
			else
				j++;
		}
		i++;
	}
	return (NULL);
}
