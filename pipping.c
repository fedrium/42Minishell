#include "minishell.h"

//sort out fd and pipes before fork
//wait for child to complete before next

void    print_args(t_list *head_tokens)
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

void	print_cp(t_cp *head_cp)
{
	t_cp	*node;
	int		i;

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

//creates child processes and pipes,
//run the functions in the child process
// !!!need to add multiple pipes!!!

t_cp	*make_cp(t_list *head_tokens)
{
	t_cp 	*cp_head;
	t_cp	*cp_node;
	int		ori_stdout;
	int		ori_stdin;

	dprintf(2, "error1\n");
	cp_head = malloc(sizeof(t_cp));
	dprintf(2, "error2\n");
	if (cp_head == NULL)
		printf("error3\n");
	cp_head->next = NULL;
	dprintf(2, "error4\n");
	split_args(&cp_head->tokens, &head_tokens);
	dprintf(2, "error5\n");
	cp_node = cp_head;
	// printf("first node\n");
	// print_args(cp_node->tokens);
	while (head_tokens != NULL)
	{
		dprintf(2, "error6\n");
		cp_node->next = malloc(sizeof(t_cp));
		if (cp_node->next == NULL)
			printf("error7\n");
		cp_node->next->next = NULL;
		split_args(&cp_node->next->tokens, &head_tokens);
		dprintf(2, "error8\n");
		cp_node = cp_node->next;
		dprintf(2, "error9\n");
	}
	dprintf(2, "error10\n");
	cp_node = cp_head;
	// print_cp(cp_head);
	ori_stdout = dup(STDOUT_FILENO);
	ori_stdin = dup(STDIN_FILENO);
	//when pipe runs, tokenize run twice, why?
	pipe(cp_node->pipe);
	dup2(cp_node->pipe[0], STDIN_FILENO);
	dup2(cp_node->pipe[1], STDOUT_FILENO);
	close(cp_node->pipe[0]);
	close(cp_node->pipe[1]);
	while (cp_node->next != NULL)
	{
		dprintf(2, "erro11r\n");
		// pipe(cp_node->pipe);
		dprintf(2, "error12\n");
		pipe(cp_node->next->pipe);
		dprintf(2, "error13\n");
		dup2(cp_node->next->pipe[1], cp_node->pipe[1]);
		dprintf(2, "error14\n");
		close(cp_node->next->pipe[1]);
		dprintf(2, "error15\n");
		// segfault here
		// close(cp_node->next->next->pipe[0]);
		dprintf(2, "error16\n");
		cp_node = cp_node->next;
		dprintf(2, "error17\n");
	}
	dup2(ori_stdout, STDOUT_FILENO);
	close(ori_stdout);
	dup2(ori_stdin, STDIN_FILENO);
	close(ori_stdin);
	close(cp_node->pipe[0]);
	close(cp_node->pipe[1]);
	dprintf(2, "error 18\n");
	return (cp_head);
}

void     organise_args(t_list *head_tokens, t_list *head_env)
{
	t_cp	*child_processes;
	pid_t	pid;
	
	pid = 1;
	child_processes = make_cp(head_tokens);
	// dprintf(2, "error bitch!\n");
	while (child_processes != NULL)
	{
		pid = fork();
		if (pid == 0)
		{
			run_functions(child_processes->tokens, head_env);
			dprintf(2, "error 19\n");
			exit(0);
		}
		else
			child_processes = child_processes->next;
	}
	while (wait(NULL) > 0);
}

//return head of the start of the command, point last argument to NULL instead of pipe
void    split_args(t_list **segment, t_list **head_tokens)
{
	t_list  *temp; //for swaping head

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

void    run_functions(t_list *head_tokens, t_list *head_env)
{
	int size;

	size = ft_lstsize(head_tokens);
	if (ft_strncmp(((t_token *)head_tokens->content)->token, "echo", 5) == 0)
		echo(head_tokens);
	else if (ft_strncmp(((t_token *)head_tokens->content)->token, "cd", 3) == 0)
		cd(head_env, head_tokens, size);
	else if (ft_strncmp(((t_token *)head_tokens->content)->token, "pwd", 4) == 0)
		pwd();
	else if (ft_strncmp(((t_token *)head_tokens->content)->token, "env", 4) == 0)
		pr_env(head_env);
	// if (ft_strncmp(((t_token *)head_tokens)->token,, "unset", 5) == 0)
	// 	unset(head_env, ((t_token *)head_tokens->next->content)->token);
	// if	(ft_strncmp(((t_token *)head_tokens)->token,, "export", 6) == 0)
	// 	export(head_env, head_tokens);
	else if (ft_strncmp(((t_token *)head_tokens->content)->token, "export", 7) == 0)
		export(head_env, head_tokens);
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
	    get_file(head_tokens, head_env);
}