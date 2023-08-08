#include "minishell.h"

int exec_sequence(t_list *head_env, t_list *head_tokens)
{
    t_list  *tokens;

    tokens = head_env;
    //pipe for first, loop for all other.
    while (is_pipe(tokens->next) && tokens != NULL)
    {
        int fd[2];
        pipe(fd);
        if (not_last(tokens))
            dup2(1, fd[1]); //direct output to input if not the last process;
        execute_function(tokens);
        tokens = tokens->next;
    }
}