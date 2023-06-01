/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyu-xian <cyu-xian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 14:33:19 by cyu-xian          #+#    #+#             */
/*   Updated: 2022/05/09 11:38:18 by cyu-xian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	int	memsize(int n)
{
	int	mem;

	mem = 0;
	if (n == -2147483648)
		return (11);
	if (n == 0)
		return (1);
	if (n < 0)
	{
		n = n * -1;
		mem++;
	}
	while (n > 0)
	{
		n /= 10;
		mem++;
	}
	return (mem);
}

static	void	ft_putnbr_itoa(int nbr, char *str, int ctr)
{
	if (nbr == -2147483648)
	{
		str[0] = '-';
		str[1] = '2';
		nbr = 147483648;
	}
	if (nbr == 0)
		str[0] = '0';
	if (nbr < 0)
	{
		str[0] = '-';
		nbr = -nbr;
	}
	while (nbr > 0)
	{
		str[ctr--] = nbr % 10 + '0';
		nbr /= 10;
	}
}

char	*ft_itoa(int n)
{
	char	*str;
	int		mem;
	int		ctr;

	mem = memsize(n);
	str = malloc(sizeof(char) * (mem + 1));
	if (str == NULL)
		return (NULL);
	str[mem] = '\0';
	ctr = mem - 1;
	ft_putnbr_itoa(n, str, ctr);
	return (str);
}
