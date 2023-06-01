/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyu-xian <cyu-xian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/15 13:41:52 by cyu-xian          #+#    #+#             */
/*   Updated: 2022/05/09 14:21:05 by cyu-xian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	char	*dst;

	dst = (char *)malloc (count * size);
	if (dst == NULL)
		return (NULL);
	if (count > 4294967295 || size > 4294967295)
		return (NULL);
	ft_bzero(dst, (count * size));
	return (dst);
}
