#include "minishell.h"

//sort out fd and pipes before fork
//wait for child to complete before next

void    print_args(t_list *head_tokens)
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

t_cp *make_cp(t_list *head_tokens, int *out, int *in)
{
	t_cp *cp_head = NULL;
	t_cp *cp_node = NULL;
	int ori_stdout;
	int ori_stdin;

	cp_head = malloc(sizeof(t_cp));
	// if (cp_head == NULL)
	// {
	//     perror("malloc");
	//     exit(EXIT_FAILURE);
	// }
	cp_head->next = NULL;
	split_args(&cp_head->tokens, &head_tokens);
	cp_node = cp_head;
	while (head_tokens != NULL)
	{
		cp_node->next = malloc(sizeof(t_cp));
		// if (cp_node->next == NULL)
		// {
		//     perror("malloc");
		//     exit(EXIT_FAILURE);
		// }
		cp_node->next->next = NULL;
		split_args(&cp_node->next->tokens, &head_tokens);
		// print_cp(cp_node);
		cp_node = cp_node->next;
	}
	ori_stdout = dup(STDOUT_FILENO);
	ori_stdin = dup(STDIN_FILENO);

	// // the HEAD does not nEED a pipe

	// pre-piping the cp will make cat | cat | ls fail

	// reason? idk honestly
	// probably cause the fd cant close fast enough?
	// or maybe theres a fd leaking somewhere that idk of :P

	// update, its cause if you prepipe, you need to close ALL the write and read end for
	// ALL pipes that arent used
	// so say there is 4 command
	// when you run the 1st command, you need to close the 3rd and 4th one's pipe
	// running the 2nd one requires you to close the 4th one's pipe
	// and so on

	// the reason why you dont need to close the previous one is cause you already closed em
	// before executing the next cp

	// cp_node = cp_head->next;

	// while (cp_node != NULL)
	// {
	// 	pipe(cp_node->pipe);
	// 	cp_node = cp_node->next;
	// }

	(*out) = ori_stdout;
	(*in) = ori_stdin;
	return (cp_head);
}

void organise_args(t_list *head_tokens, t_list **head_env)
{
	t_cp *child_processes;
	t_cp *head;
	t_cp *temp;
	pid_t pid;
	int ori_stdout;
	int ori_stdin;
	int num_processes;

	num_processes = 0; // Count the number of child processes
	pid = 1;
	child_processes = make_cp(head_tokens, &ori_stdout, &ori_stdin);
	head = child_processes;
	while (child_processes != NULL)
	{
		num_processes++; // Increment the number of child processes

		// heres a better way to do pumbling
		// instead of prepiping, only pipe if there is a next command that requires pipe

		// cat | cat | ls will work if you pipe it like this instead

		if (child_processes->next)
			pipe(child_processes->next->pipe);
		// pipe(child_processes->next->pipe);
		pid = fork();
		if (pid == 0)
		{
			// Redirect STDIN to read from its own pipe

			if (num_processes)
			{
				dup2(child_processes->pipe[0], STDIN_FILENO);
				close(child_processes->pipe[0]); // Close read end of its own pipe
			}

			// if (child_processes->next == NULL)
			// {
			// 	// Redirect STDOUT to the original STDOUT
			// 	dup2(ori_stdout, STDOUT_FILENO);
			// 	close(ori_stdout); // Close the original STDOUT
			// }

			if (child_processes->next != NULL)
			{
				// pipe(child_processes->pipe);
				// gotta close the READ end of the next child_process
				// or else its gonna assume someone is still using the read end
				close(child_processes->next->pipe[0]);
				// Redirect STDOUT to write to the pipe of the next child process
				dup2(child_processes->next->pipe[1], STDOUT_FILENO);
				close(child_processes->next->pipe[1]); // Close write end of the next pipe
			}

			// i have removed the big chunk o' comment here :P

			// preetty sure if everything is done properly,
			// theres no need to close the write end
			// close(child_processes->pipe[1]);
			run_functions(child_processes->tokens, head_env);
			exit(0);
		}
		else
		{
			// 0 is read end of pipe
			// 1 is write end of pipe
			if (num_processes)
				close(child_processes->pipe[0]);

			if (child_processes->next != NULL)
				close(child_processes->next->pipe[1]);

			child_processes = child_processes->next;
		}
	}
	// Wait for all child processes to exit
	temp = head;
	while (num_processes > 0)
	{
		waitpid(0, NULL, 0);

		// big chunk of commented out code removed here as well

		num_processes--;
	}
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

void    run_functions(t_list *head_tokens, t_list **head_env)
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
	// if (ft_strncmp(((t_token *)head_tokens)->token,, "unset", 5) == 0)
	// 	unset(head_env, ((t_token *)head_tokens->next->content)->token);
	// if	(ft_strncmp(((t_token *)head_tokens)->token,, "export", 6) == 0)
	// 	export(head_env, head_tokens);
	else if (ft_strncmp(((t_token *)head_tokens->content)->token, "export", 7) == 0)
	{
		export(head_env, head_tokens);
	}
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