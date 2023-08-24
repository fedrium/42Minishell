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

//creates child processes and pipes,
//run the functions in the child process
void     execute_args(t_list *head_tokens, t_list *head_env)
{
	t_list  *segment;
	t_list  **seg_addr;
	pid_t     pid;
	int     fd[2];
	int     original_stdout;
	int     last;
	int     child_num;

	//create the pipe
	pipe(fd);
	//keep track of number of childs
	child_num = 0;
	//preserve the original stdout
	original_stdout = dup(1);
	//init pid for parent process
	pid = 1;
	//init command segment
	segment = NULL;
	//redirect output into pipe
	dup2(fd[1], 1);
	//split long linked list into seperate segments of small linked lists,
	//pass them into the child process and free them immediately after that.
	while (head_tokens != NULL)
	{
		//check if the current segment is the last arguement
		last = is_last_arg(head_tokens);
		//split the linked list
		split_args(&segment, &head_tokens);
		//store address of seperated link list to free later
		seg_addr = &segment;
		//if is parent
		if (pid > 0)
		{
			child_num++;
			pid = fork();
			// if (pid > 0)
				// lst_free_all(segment);
		}
		if (pid == 0)
		{
			//print output to stdout if is last command
			if (last)
				dup2(original_stdout, 1);
			run_functions(segment, head_env);
			// free(*seg_addr);
			exit(0);
		}
	}
	//wait for all child to end
	if (pid > 0)
	{
		while(child_num > 0)
		{
			waitpid(0, NULL, 0);
			child_num--;
		}
		//redirect output so that minishell shows at prompt
		dup2(original_stdout, 1);
	}
}

//return head of the start of the command, point last argument to NULL instead of pipe
void    split_args(t_list **segment, t_list **head_tokens)
{
	t_list  *temp; //for swaping head

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