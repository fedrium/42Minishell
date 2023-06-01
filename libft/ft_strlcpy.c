/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyu-xian <cyu-xian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/06 10:56:28 by cyu-xian          #+#    #+#             */
/*   Updated: 2022/05/09 12:03:09 by cyu-xian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	i;

	i = 0;
	while (dstsize > i && src[i] != '\0')
	{
		dst[i] = src[i];
		i++;
	}
	if (i == dstsize)
		i--;
	if (i < dstsize)
		dst[i] = '\0';
	return (ft_strlen (src));
}
