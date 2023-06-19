/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalee <yalee@student.42.fr.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 10:23:52 by yalee             #+#    #+#             */
/*   Updated: 2023/06/20 01:52:04 by yalee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*tokenize(char *line)
{
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
	int		in_quote;
	char	*token;
	char	quote;

	in_quote = -1;
	token = NULL;
	quote = 0;
	while (line[*p] == ' ')
		*p += 1;
	while (line[*p] != ' ' || in_quote > 0)
	{
		if (!line[*p])
			break;
		if (is_quote(line[*p], quote))
		{
			quote = line[*p];
			in_quote *= -1;
		}
		token = ft_joinchar(token, line[*p]);
		*p += 1;
	}
	if (in_quote > 0)
		missing_quote(&token, quote);
	return (token);
}

void	remove_quotes(char **token, char q)
{
	int		i;
	int		j;
	char	*temp;
	
	j = 0;
	i = 0;
	temp = count_quotes(token, q);
	while (temp[i])
	{
		if (temp[i] != q)
		{
			(*token)[j] = temp[i];
			j++;
		}
		i++;
	}
	(*token)[j] = '\0';
	free(temp);
}

char	*count_quotes(char **token, char q)
{
	char *temp;
	int q_num;
	int i;

	i = 0;
	q_num = 0;
	temp = ft_strdup(*token);
	while (temp[i])
	{
		if (temp[i] == q)
			q_num++;
		i++;
	}
	free(*token);
	*token = (char *)malloc((sizeof(char) * ft_strlen(temp)) - q_num + 1);
	return (temp);
}

int	is_quote(char c, char quote)
{
	if (quote == 0)
		return (1);
	if (c == quote)
		return (1);
	return (0);	
}

char	*ft_joinchar(char *token, char c)
{
	// printf("joinchar\n");
	char	*new_str;
	int		i;
	int		len;

	if (token == NULL)
	{
		new_str = (char *)malloc(sizeof(char) + 1);
		new_str[0] = c;
		new_str[1] = '\0';
		return (new_str);
	}
	i = 0;
	len = ft_strlen(token) + 1;
	new_str = (char *)malloc(sizeof(char) * len + 1);
	while (i < len - 1)
	{
		new_str[i] = token[i];
		i++; 
	}
	new_str[i] = c;
	return (new_str);
}

void	missing_quote(char **token, char quote)
{
	char	*line;

	line = NULL;
	while (!quote_in_line(line, quote))
	{
		line = readline("> ");
		*token = ft_joinchar(*token, '\n');
		*token = ft_strjoin(*token, line);
		free(line);
	}
	remove_quotes(token, quote);
}



int	quote_in_line(char *line, char quote)
{
	int	i;

	i = 0;
	while (line != NULL && line[i])
	{
		if (line[i] == quote)
			return(1);
		i++;
	}
	return (0);
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