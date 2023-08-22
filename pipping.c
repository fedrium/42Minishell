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

void     execute_args(t_list *head_tokens, t_list *head_env)
{
    t_list  *segment;
    t_list  **seg_addr;
    pid_t     pid;
    int     fd[2];
    int     original_stdout;
    int     last;
    int     child_num;

    pipe(fd);
    child_num = 0;
    original_stdout = dup(1);
    pid = 1;
    segment = NULL;
    dup2(fd[1], 1);
    // (void)head_env;
    while (head_tokens != NULL)
    {
        last = is_last_arg(head_tokens);
        split_args(&segment, &head_tokens);
        seg_addr = &segment;
        if (pid > 0)
        {
            child_num++;
            pid = fork();
            if (pid > 0)
                lst_free_all(segment);
        }
        if (pid == 0)
        {
            if (last)
                dup2(original_stdout, 1);
            printf("%d\n", last);
            run_functions(segment, head_env);
            // printf("out\n");
            free(*seg_addr);
            exit(0);
        }
    }
    if (pid > 0)
    {
        while(child_num > 0)
        {
            waitpid(0, NULL, 0);
            child_num--;
        }
        // lst_free_all(segment);
        dup2(original_stdout, 1);
    }
}

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

    // print_args(head_tokens);
    size = ft_lstsize(head_tokens);
    if (ft_strncmp(((t_token *)head_tokens)->token, "echo", 4) == 0)
        echo(head_tokens);
    if (ft_strncmp(((t_token *)head_tokens)->token, "cd", 2) == 0)
        cd(head_env, head_tokens, size);
    if (ft_strncmp(((t_token *)head_tokens)->token, "pwd", 3) == 0)
        pwd();
    if (ft_strncmp(((t_token *)head_tokens)->token, "env", 3) == 0)
        pr_env(head_env);
    // if (ft_strncmp(((t_token *)head_tokens)->token,, "unset", 5) == 0)
    // 	unset(head_env, ((t_token *)head_tokens->next->content)->token);
    // if	(ft_strncmp(((t_token *)head_tokens)->token,, "export", 6) == 0)
    // 	export(head_env, head_tokens);
    if (ft_strncmp(((t_token *)head_tokens->content)->token, "export", 6) == 0)
        export(head_env, head_tokens);
    if (ft_strncmp(((t_token *)head_tokens->content)->token, "test", 4) == 0)
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
}