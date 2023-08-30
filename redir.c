#include "minishell.h"

void	redir_app(t_list *head_tokens, t_list *head_env)
{
	int	fd;
	int	stdo;
	// int	stdi;

	stdo = dup(1);
	fd = open("test", O_CREAT | O_APPEND | O_WRONLY, 0644);
	run_functions(head_tokens, head_env);
	dup2(fd, 1);
	close(fd);
	dup2(stdo, 1);
	// write(fd, "test", 4);
}

void	redir_check(t_list *head_tokens, t_list *head_env)
{
	t_list	*node;
	t_list	*head;

	head = head_tokens;
	while (head_tokens->next != NULL)
	{
		node = head_tokens;
		// printf("line: %s\n", ((t_token*)node->content)->token);
		if (ft_strncmp((((t_token*)node->content)->token), ">>", 3) == 0)
		{
			// printf(">>\n");
			redir_app(head, head_env);
			
		}
		else if (ft_strncmp((((t_token*)node->content)->token), ">", 2) == 0)
		{
			// printf(">\n");
		}
		head_tokens = head_tokens->next;
	}
}