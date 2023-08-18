#include "minishell.h"

//sort out fd and pipes before fork
//wait for child to complete before next
int execute_args(t_list *head_tokens, t_list *head_env)
{
    t_list  *temp;
    t_list  *temp_head;
    int     pid;
    int     fd[2];
    int     original_stdout;

    pipe(fd);
    original_stdout = dup(stdout);
    temp_head = head_tokens;
    while (head_tokens != NULL)
    {
        split_args(temp, head_tokens);
        create_pipes_and_forks(temp, fd, original_stdout, is_last_args(temp_head));
    }
}

void    create_pipes_and_forks(t_list *temp, int original_stdout, int *fd, int flag)
{
    int pid;

    pid = fork()
}

void    split_args(t_list *temp, t_list *temp_head)
{
    t_list  *node;

    node = temp_head;
    while (node != NULL && strcncmp(((t_token *)node->content)->token, "|", 2) != 0)
        node = node->next;
    temp = node;
    temp_head = node->next;
}

int is_last_srgs(t_list *head_tokens)
{
    t_list  *node;

    node = head_tokens;
    while (node != NULL)
    {
        if (strncmp(((t_token *)node->content)->token, "|", 2) == 0)
            return (0);
        node = node->next;
    }
    return (1);
}

void    create_pipes_and_forks(t_list *temp, int *fd, int backup_stdout, int flag)
{
    dup2(fd[1], stdout);
    fork();
    if (flag == 1)
        dup2(backup_stdout, 1);
}

void    run_functions(t_list *head_tokens, t_list *head_env)
{
    int size;

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