#include "minishell.h"

char    *get_tl_str(t_list *node)
{
    return(((t_token *)node->content)->token);
}

void    lst_free_one(t_list *node)
{
    free(((t_token *)node->content)->token);
    // free(node->content);
    free(node);
}

void    lst_free_env(t_list *head_env)
{
    while (head_env != NULL)
    {
        free(((t_env *)head_env->content)->key);
        free(((t_env *)head_env->content)->value);
        head_env = head_env->next;
    }
}

void    lst_free_all(t_list *head)
{
    while (head != NULL)
    {
        lst_free_one(head);
        head = head->next;
    }
}

int is_last_arg(t_list *head_tokens)
{
    t_list *p;

    p = head_tokens;
    while (p != NULL)
    {
        printf("%s\n", ((t_token *)p->content)->token);
        if (strncmp(((t_token *)p->content)->token, "|", 1) == 0 && p->next != NULL)
            return (0);
        p = p->next;
    }
    return (1);
}

char    is_special(t_list *node)
{
    if (strncmp(((t_token *)node->content)->token, "|", 2) == 0)
        return ('|');
    else
        return (0);
}