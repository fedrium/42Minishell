/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalee <yalee@student.42.fr.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 10:23:52 by yalee             #+#    #+#             */
/*   Updated: 2023/06/19 16:13:58 by yalee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*tokenize(char *line)
{
	// printf("tokenize\n");
	t_list	*head;
	t_list	*node;
	int		p;

	p = 0;
	node = ft_lstnew((void *)get_token(line, &p));
	head = node;
	while (p < (int)ft_strlen(line) && !is_last(line, &p))
	{
		node->next = ft_lstnew((void *)get_token(line, &p));
		node = node->next;
		
	}
	return (head);
}

char	*get_token(char *line, int *p)
{
	// printf("get_token\n");
	int		len;
	int		i;
	char	*token;

	i = 0;
	len = get_token_len(line, p);
	token = (char *)malloc((sizeof(char) * len) + 1);
	while (line[*p] != ' ' && line[*p])
	{
		token[i] = line[*p];
		i++;
		*p = *p + 1;
	}
	token[i] = '\0';
	return (token);
}

//removes spaces at the same time
int	get_token_len(char *line, int *p)
{
	// printf("len\n");
	int	i;
	
	while (line[*p] == ' ')
		*p = *p + 1;
	i = *p;
	while (line[i] != ' ' && line[i])
		i++;
	// printf("%i\n", i - *p);
	return (i - *p);
}

int	is_last(char *line, int *p)
{
	int	i;
	
	i = *p;
	while(line[i] == ' ')
		i++;
	if (line[i])
		return (0);
	return (1);
}