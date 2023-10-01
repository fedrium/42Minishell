/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalee <yalee@student.42.fr.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 15:35:36 by yalee             #+#    #+#             */
/*   Updated: 2023/10/01 23:56:54 by yalee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	redir_heredoc(t_list *head, t_list *start, int fd)
{
	t_list	*temp;
	char	*file_name;

	file_name = (((t_token *)head->next->content)->token);
	heredoc(file_name);
	temp = head->next;
	redir_rm(&start, head);
	redir_rm(&start, temp);
	head = start;
	if (start->next != NULL)
	{
		dup2(fd, 0);
		close(fd);
	}
	fd = open(".temp", O_RDONLY, 0777);
	return (fd);
}

void	put_line(int fd, char *line)
{
	write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
	free(line);
}

void	heredoc(char *delimeter)
{
	int		fd;
	char	*line;
	pid_t	pid;

	fd = open(".temp", O_WRONLY | O_CREAT | O_TRUNC, 0777);
	signal(SIGQUIT, SIG_IGN);
	pid = 1;
	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, cntl_c);
		while (1)
		{
			line = readline("heredoc>");
			if (line == NULL)
			{
				printf("here\n");
				exit(0);
			}
			if (ft_strncmp(line, delimeter, ft_strlen(delimeter) + 1) == 0)
			{
				close(fd);
				free(line);
				exit(0);
			}
			put_line(fd, line);
		}
	}
	else
		waitpid(0, NULL, 0);
		printf("waited\n");
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
}
