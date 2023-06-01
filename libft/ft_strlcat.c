/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyu-xian <cyu-xian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/06 11:23:15 by cyu-xian          #+#    #+#             */
/*   Updated: 2022/05/09 15:41:43 by cyu-xian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	dstlen;
	size_t	src_iter;
	size_t	dst_iter;
	int		cat_count;

	dstlen = ft_strlen(dst);
	src_iter = 0;
	dst_iter = dstlen;
	cat_count = dstsize - dstlen - 1;
	if (dstsize > 2147483647)
		cat_count = 2147483647;
	if (dstsize == 0)
		return (ft_strlen(src));
	if (dstlen > dstsize)
		return (ft_strlen(src) + dstsize);
	if (cat_count < 1)
		return (ft_strlen(src) + dstlen);
	while (src_iter < (size_t) cat_count && src[src_iter])
		dst[dst_iter++] = src[src_iter++];
	dst[dst_iter] = 0;
	return (dstlen + ft_strlen(src));
}
