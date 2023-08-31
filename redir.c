#include "minishell.h"
//>>
void	redir_app(t_list *head_tokens, t_list *head_env, char *file_name)
{
	int	fd;
	int	stdo;

	stdo = dup(1);
	fd = open(file_name, O_CREAT | O_APPEND | O_WRONLY, 0777);
	dup2(fd, 1);
	run_functions(head_tokens, head_env);
	close(fd);
	dup2(stdo, 1);
}
//> .temp >> 
void	redir_rep(t_list *head_tokens, t_list *head_env, char *file_name)
{
	int	fd;
	int	stdo;

	stdo = dup(1);
	fd = open(file_name, O_CREAT | O_TRUNC | O_WRONLY, 0777);
	dup2(fd, 1);
	run_functions(head_tokens, head_env);
	close(fd);
	dup2(stdo, 1);
}

void	redir_in(t_list *head_tokens, t_list *head_env, char *file_name)
{
	int	fd;

	fd = open(file_name, O_RDONLY, 0777);
	dup2(fd, 0);
	run_functions(head_tokens, head_env);
	close(fd);
}



void	redir_rm(t_list **head, t_list *to_del)
{
	t_list	*temp;
	t_list	*prev;
	// t_list	*node;

	prev = NULL;	
	temp = *head;
	while(temp != NULL)
	{
		// node = temp;
		if (temp == to_del)
		{
			prev->next = temp->next;
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

// echo hi >> test

int	redir_check(t_list *head_tokens, t_list *head_env)
{
	t_list	*head;
	t_list	*start;
	t_list	*temp;
	char	*file_name;

	(void)head_env;
	start = head_tokens;
	head = head_tokens;
	temp = head_tokens;
	while (head != NULL)
	{
		if (ft_strncmp((((t_token*)head->content)->token), ">>", 3) == 0)
		{
			file_name = (((t_token*)head->next->content)->token);
			temp = head->next;
			redir_rm(&start, head);
			redir_rm(&start, temp);
			ll_iter(start);
			redir_app(start, head_env, file_name);
			if (head != NULL)
				continue;
			return (1);
		}
		head = head->next;
	}
	// redir_rm(&head_tokens, head_tokens->next);
	
	return (0);
}