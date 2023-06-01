/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyu-xian <cyu-xian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/19 13:26:32 by cyu-xian          #+#    #+#             */
/*   Updated: 2022/05/09 15:15:05 by cyu-xian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	i;
	size_t	j;
	size_t	k;
	size_t	l;
	char	*str;

	i = ft_strlen((char *)s1);
	j = ft_strlen((char *)s2);
	k = -1;
	l = 0;
	str = (char *)malloc (i + j + 1);
	if (str == NULL)
		return (NULL);
	while (i > ++k)
		str[k] = s1[k];
	while (j > l)
	{
		str[k] = s2[l];
		k++;
		l++;
	}
	str[k] = '\0';
	return (str);
}
