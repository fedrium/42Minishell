/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalee <yalee@student.42.fr.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 15:35:21 by yalee             #+#    #+#             */
/*   Updated: 2023/10/01 22:59:22 by yalee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	redir_app(t_list *head, t_list *start, int fd)
{
	t_list	*temp;
	char	*file_name;

	file_name = (((t_token *)head->next->content)->token);
	if (fd != 0)
		close(fd);
	fd = open(file_name, O_CREAT | O_APPEND | O_WRONLY, 0777);
	temp = head->next;
	redir_rm(&start, head);
	redir_rm(&start, temp);
	return (fd);
}

int	redir_rep(t_list *head, t_list *start, int fd)
{
	t_list	*temp;
	char	*file_name;

	file_name = (((t_token *)head->next->content)->token);
	if (fd != 0)
		close(fd);
	fd = open(file_name, O_CREAT | O_TRUNC | O_WRONLY, 0777);
	temp = head->next;
	if (ft_strncmp((((t_token *)start->content)->token), ">", 2) == 0)
	{
		dprintf(2, "here\n");
		return (0);
	}
	redir_rm(&start, head);
	redir_rm(&start, temp);
	return (fd);
}

int	redir_in(t_list *head, t_list *start, int fd)
{
	t_list	*temp;
	char	*file_name;

	file_name = (((t_token *)head->next->content)->token);
	if (fd != 0)
		close(fd);
	fd = open(file_name, O_RDONLY, 0777);
	temp = head->next;
	redir_rm(&start, head);
	redir_rm(&start, temp);
	return (fd);
}

void	find_and_run_process(t_list **head, t_list **start, int *fd, int *redir)
{
	if (ft_strncmp((((t_token *)(*head)->content)->token), ">>", 3) == 0)
	{
		(*redir) = 1;
		(*fd) = redir_app((*head), (*start), (*fd));
		(*head) = (*start);
	}
	else if (ft_strncmp((((t_token *)(*head)->content)->token), ">", 2) == 0)
	{
		(*redir) = 1;
		dprintf(2, "head:\n");
		print_tokens(*head);
		dprintf(2, "start\n");
		print_tokens(*start);
		(*fd) = redir_rep((*head), (*start), (*fd));
		(*head) = (*start);
	}
	else if (ft_strncmp((((t_token *)(*head)->content)->token), "<", 2) == 0)
	{
		(*redir) = 2;
		(*fd) = redir_in((*head), (*start), (*fd));
		(*head) = (*start);
	}
	else if (ft_strncmp((((t_token *)(*head)->content)->token), "<<", 3) == 0)
	{
		(*redir) = 2;
		(*fd) = redir_heredoc((*head), (*start), (*fd));
		(*head) = (*start);
	}
}

int	redir_check(t_list *head_tokens, t_main_vars *main_vars)
{
	t_list	*head;
	t_list	*start;
	int		fd;
	int		redir;

	start = head_tokens;
	head = head_tokens;
	fd = 0;
	while (head != NULL)
	{
		find_and_run_process(&head, &start, &fd, &redir);
		head = head->next;
	}
	if (redir == 1)
	{
		dup2(fd, 1);
		close(fd);
	}
	else if (redir == 2)
	{
		dup2(fd, 0);
		close(fd);
	}
	if (redir == 1 || redir == 2)
		return (1);
	return (0);
}
