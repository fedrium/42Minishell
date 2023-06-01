/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyu-xian <cyu-xian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/28 14:26:51 by cyu-xian          #+#    #+#             */
/*   Updated: 2022/05/11 13:54:35 by cyu-xian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	int	cnt_word(char const *str, char c)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	if (c == 0)
		return (1);
	while (str[i] != '\0')
	{
		while (str[i] == c)
			i++;
		if (str[i] != c && str[i])
			j++;
		while (str[i] != c && str[i])
			i++;
	}
	return (j);
}

int	check_null_s(char const *s, char ***res)
{
	if (!*s)
	{
		*res[0] = 0;
		return (1);
	}
	return (0);
}

static void	traverse_start(int *start_dex, int *end_dex, char const *s, char c)
{
	while (s[*start_dex] == c)
		*start_dex = *start_dex + 1;
	while (s[*start_dex + *end_dex] != c && s[*start_dex + *end_dex] != '\0')
		*end_dex = *end_dex + 1;
}

char	**ft_split(char const *s, char c)
{
	char	**res;
	int		total_words;
	int		word_dex;
	int		start_dex;
	int		end_dex;

	total_words = cnt_word(s, c);
	res = (char **)malloc(sizeof(char *) * (total_words + 1));
	if (!res)
		return (0);
	res[total_words] = 0;
	if (check_null_s(s, &res))
		return (res);
	start_dex = 0;
	word_dex = 0;
	while (word_dex < total_words)
	{
		end_dex = 0;
		traverse_start(&start_dex, &end_dex, s, c);
		res[word_dex] = ft_substr(s, start_dex, end_dex);
		start_dex += end_dex;
		word_dex++;
	}
	return (res);
}
