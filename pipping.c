#include "minishell.h"

void run_functions_nopp(t_list *head_tokens, t_list **head_env)
{
	int size;

	size = ft_lstsize(head_tokens);
	if (ft_strncmp(((t_token *)head_tokens->content)->token, "echo", 5) == 0)
		echo(head_tokens);
	else if (ft_strncmp(((t_token *)head_tokens->content)->token, "cd", 3) == 0)
		cd(*head_env, head_tokens, size);
	else if (ft_strncmp(((t_token *)head_tokens->content)->token, "pwd", 4) == 0)
		pwd();
	else if (ft_strncmp(((t_token *)head_tokens->content)->token, "env", 4) == 0)
		pr_env(*head_env);
	else if (ft_strncmp(((t_token *)head_tokens->content)->token, "export", 7) == 0)
		export(head_env, head_tokens);
	else if (ft_strncmp(((t_token *)head_tokens->content)->token, "error", 6) == 0)
		printf("error code: %d\n", g_ercode);
	else if (ft_strncmp(((t_token *)head_tokens->content)->token, "exit", 5) == 0)
		exit_func(head_tokens, *head_env);
	else if (ft_strncmp(((t_token *)head_tokens->content)->token, "test", 5) == 0)
	{
		t_list *node;
		node = head_tokens;
		printf("tout: %s\n", ((t_token *)node->content)->token);
		while (node->next != NULL)
		{
			printf("tout: %s\n", ((t_token *)node->next->content)->token);
			if (node->next != NULL)
				node = node->next;
		}
	}
	else
		get_file_nopp(head_tokens, *head_env);
}

void print_args(t_list *head_tokens)
{
	t_list *node;
	node = head_tokens;
	dprintf(2, "tout: %s\n", ((t_token *)node->content)->token);
	while (node->next != NULL)
	{
		dprintf(2, "tout: %s\n", ((t_token *)node->next->content)->token);
		if (node->next != NULL)
			node = node->next;
	}
}

void print_cp(t_cp *head_cp)
{
	t_cp *node;
	int i;

	i = 1;
	node = head_cp;
	while (node != NULL)
	{
		printf("node: %d\n", i);
		print_args(node->tokens);
		i++;
		node = node->next;
	}
}

t_cp *make_cp(t_list *head_tokens, int *out, int *in)
{
	t_cp *cp_head = NULL;
	t_cp *cp_node = NULL;
	int ori_stdout;
	int ori_stdin;

	cp_head = malloc(sizeof(t_cp));
	cp_head->next = NULL;
	split_args(&cp_head->tokens, &head_tokens);
	cp_node = cp_head;
	while (head_tokens != NULL)
	{
		cp_node->next = malloc(sizeof(t_cp));
		cp_node->next->next = NULL;
		split_args(&cp_node->next->tokens, &head_tokens);
		cp_node = cp_node->next;
	}
	ori_stdout = dup(STDOUT_FILENO);
	ori_stdin = dup(STDIN_FILENO);
	(*out) = ori_stdout;
	(*in) = ori_stdin;
	return (cp_head);
}

int	get_cp_size(t_cp *cp)
{
	int i;

	i = 0;
	while(cp != NULL)
	{
		cp = cp->next;
		i++;
	}
	return (i);
}

void organise_args(t_list *head_tokens, t_list **head_env)
{
	t_cp *child_processes;
	t_cp *cp_head;
	pid_t pid;
	int ori_stdout;
	int ori_stdin;
	int num_processes;

	num_processes = 0; // Count the number of child processes
	child_processes = make_cp(head_tokens, &ori_stdout, &ori_stdin);
	cp_head = child_processes;
	pid = 1;
	if (child_processes->next == NULL)
	{
		run_functions(head_tokens, head_env);
		child_processes = child_processes->next;
	}
	while (child_processes != NULL)
	{
		num_processes++; // Increment the number of child processes
		if (child_processes->next)
			pipe(child_processes->next->pipe);
		pid = fork();
		if (pid == 0)
		{
			if (num_processes)
			{
				dup2(child_processes->pipe[0], STDIN_FILENO);
				close(child_processes->pipe[0]); // Close read end of its own pipe
			}
			if (child_processes->next != NULL)
			{
				close(child_processes->next->pipe[0]);
				dup2(child_processes->next->pipe[1], STDOUT_FILENO);
				close(child_processes->next->pipe[1]); // Close write end of the next pipe
			}
			run_functions_nopp(child_processes->tokens, head_env);

			exit(0);
		}
		else
		{
			if (num_processes)
				close(child_processes->pipe[0]);

			if (child_processes->next != NULL)
				close(child_processes->next->pipe[1]);

			child_processes = child_processes->next;
		}
	}
	// Wait for all child processes to exit
	while (num_processes > 0)
	{
		waitpid(0, &g_ercode, 0);
		g_ercode = g_ercode % 255;
		num_processes--;
	}
	free_cp(cp_head);
}

void	free_cp(t_cp *head)
{
	t_cp	*temp_cp;

	while (head != NULL)
	{
		printf("free1\n");
		lst_free_all(head->tokens);
		temp_cp = head;
		head = head->next;
		free(temp_cp);
	}
}
// return head of the start of the command, point last argument to NULL instead of pipe
void split_args(t_list **segment, t_list **head_tokens)
{
	t_list *temp; // for swaping head

	(*segment) = malloc(sizeof(t_list));
	*segment = *head_tokens;
	while ((*head_tokens)->next != NULL && is_special((*head_tokens)->next) != '|')
		(*head_tokens) = (*head_tokens)->next;
	if ((*head_tokens)->next != NULL && (*head_tokens)->next->next != NULL)
	{
		temp = (*head_tokens)->next->next;
		lst_free_one((*head_tokens)->next);
		(*head_tokens)->next = NULL;
		(*head_tokens) = temp;
		return;
	}
	(*head_tokens) = (*head_tokens)->next;
}

void run_functions(t_list *head_tokens, t_list **head_env)
{
	int size;

	size = ft_lstsize(head_tokens);
	if (ft_strncmp(((t_token *)head_tokens->content)->token, "echo", 5) == 0)
		echo(head_tokens);
	else if (ft_strncmp(((t_token *)head_tokens->content)->token, "cd", 3) == 0)
		cd(*head_env, head_tokens, size);
	else if (ft_strncmp(((t_token *)head_tokens->content)->token, "pwd", 4) == 0)
		pwd();
	else if (ft_strncmp(((t_token *)head_tokens->content)->token, "env", 4) == 0)
		pr_env(*head_env);
	else if (ft_strncmp(((t_token *)head_tokens->content)->token, "export", 7) == 0)
		export(head_env, head_tokens);
	else if (ft_strncmp(((t_token *)head_tokens->content)->token, "error", 6) == 0)
		printf("error code: %d\n", g_ercode);
	else if (ft_strncmp(((t_token *)head_tokens->content)->token, "exit", 5) == 0)
		exit_func(head_tokens, *head_env);
	else if (ft_strncmp(((t_token *)head_tokens->content)->token, "test", 5) == 0)
	{
		t_list *node;
		node = head_tokens;
		printf("tout: %s\n", ((t_token *)node->content)->token);
		while (node->next != NULL)
		{
			printf("tout: %s\n", ((t_token *)node->next->content)->token);
			if (node->next != NULL)
				node = node->next;
		}
	}
	else
		get_file(head_tokens, *head_env);
}