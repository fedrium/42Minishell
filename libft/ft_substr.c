/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyu-xian <cyu-xian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/19 13:16:26 by cyu-xian          #+#    #+#             */
/*   Updated: 2022/05/13 11:34:07 by cyu-xian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	char	*str;

	i = 0;
	if (s == 0)
		return (0);
	if (start >= ft_strlen(s))
		len = 0;
	else if (start + len >= ft_strlen(s))
		len = ft_strlen(s) - start;
	str = (char *)malloc (len + 1);
	if (str == NULL)
		return (NULL);
	while (len > i && s[start] != '\0')
	{
		str[i] = s[start];
		i++;
		start++;
	}
	str[i] = '\0';
	return (str);
}
