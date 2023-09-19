#include "minishell.h"
//>>
int	redir_app(t_list *head, t_list *start, int fd)
{
	t_list	*temp;
	char	*file_name;

	file_name = (((t_token*)head->next->content)->token);
	if (fd != 0)
		close(fd);
	fd = open(file_name, O_CREAT | O_APPEND | O_WRONLY, 0777);
	temp = head->next;
	redir_rm(&start, head);
	redir_rm(&start, temp);
	return(fd);
}
//> .temp >> 

int	redir_rep(t_list *head, t_list *start, int fd)
{
	t_list	*temp;
	char	*file_name;

	file_name = (((t_token*)head->next->content)->token);
	if (fd != 0)
		close(fd);
	fd = open(file_name, O_CREAT | O_TRUNC | O_WRONLY, 0777);
	temp = head->next;
	redir_rm(&start, head);
	redir_rm(&start, temp);
	return(fd);
}

int	redir_in(t_list *head, t_list *start, int fd)
{
	t_list	*temp;
	char	*file_name;

	file_name = (((t_token*)head->next->content)->token);
	if (fd != 0)
		close(fd);
	fd = open(file_name, O_RDONLY, 0777);
	temp = head->next;
	redir_rm(&start, head);
	redir_rm(&start, temp);
	return(fd);
}

int	redir_heredoc(t_list *head, t_list *start, int fd)
{
	t_list	*temp;
	char	*file_name;

	file_name = (((t_token*)head->next->content)->token);
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
	return(fd);
}

void	redir_rm(t_list **head, t_list *to_del)
{
	t_list	*temp;
	t_list	*prev;

	prev = NULL;	
	temp = *head;
	while(temp != NULL)
	{
		// node = temp;
		if (temp == to_del)
		{
			prev->next = temp->next;
			free(((t_token *)temp->content)->token);
			free(temp->content);
			free(temp);
			break;
		}
		prev = temp;
		temp = temp->next;
	}
}

void	ll_iter(t_list *head_tokens)
{
	t_list *head;

	while (head_tokens != NULL)
	{
		head = head_tokens;
		printf("line: %s\n", (((t_token*)head->content)->token));
		head_tokens = head_tokens->next;
	}
}

void	heredoc(char *delimeter)
{
	int 	fd;
	char	*line;

	fd = open(".temp", O_WRONLY | O_CREAT | O_TRUNC, 0777);
	while (1)
	{
		line = readline("heredoc>");
		if (line == NULL)
			return;
		if (ft_strncmp(line, delimeter, ft_strlen(delimeter) + 1) == 0)
		{
			close(fd);
			free(line);
			break;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
}

// echo hi >> test

int	redir_check(t_list *head_tokens)
{
	t_list	*head;
	t_list	*start;
	int		fd;
	int		redir; // 1 = out, 2 = in

	start = head_tokens;
	head = head_tokens;
	fd = 0;
	while (head != NULL)
	{
		if (ft_strncmp((((t_token*)head->content)->token), ">>", 3) == 0)
		{
			redir = 1;
			fd = redir_app(head, start, fd);
			head = start;
		}
		else if (ft_strncmp((((t_token*)head->content)->token), ">", 2) == 0)
		{
			redir = 1;
			fd = redir_rep(head, start, fd);
			head = start;
		}
		else if (ft_strncmp((((t_token*)head->content)->token), "<", 2) == 0)
		{
			redir = 2;
			fd = redir_in(head, start, fd);
			head = start;
		}
		else if (ft_strncmp((((t_token*)head->content)->token), "<<", 3) == 0)
		{
			redir = 2;
			fd = redir_heredoc(head ,start, fd);
			head = start;
		}
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
	return (0);
}