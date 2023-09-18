#include "minishell.h"

char    *get_tl_str(t_list *node)
{
    return(((t_token *)node->content)->token);
}

void    lst_free_one(t_list *node)
{
    free(((t_token *)node->content)->token);
    free(node->content);
    free(node);
}

void lst_free_env(t_list *head_env)
{
    t_list *temp_env;
    char    *key;
    char    *value;

    while (head_env != NULL)
    {
        // printf("free %i\n", i);
        temp_env = head_env;
        head_env = head_env->next;

        // Free key and value strings
        free(((t_env *)temp_env->content)->key);
        free(((t_env *)temp_env->content)->value);

        // Free the t_env structure itself
        free(temp_env->content);

        // Finally, free the list node
        free(temp_env);
    }
}

void    free_2dar(char **ar_2d)
{
    int i;

    i = 0;
    while (ar_2d[i])
    {
        free(ar_2d[i]);
        i++;
    }
    free(ar_2d);
}

void    lst_free_all(t_list *head)
{
    t_list  *temp;
    
    while (head != NULL)
    {
        temp = head;
        head = head->next;
        lst_free_one(temp);
    }
}

int is_last_arg(t_list *head_tokens)
{
    t_list *p;

    p = head_tokens;
    while (p != NULL)
    {
        printf("%s\n", ((t_token *)p->content)->token);
        if (ft_strncmp(((t_token *)p->content)->token, "|", 1) == 0 && p->next != NULL)
            return (0);
        p = p->next;
    }
    return (1);
}

char    is_special(t_list *node)
{
    if (ft_strncmp(((t_token *)node->content)->token, "|", 2) == 0)
        return ('|');
    else
        return (0);
}

// char	*strjoin_helper(char *str1, char *str2)
// {
// 	char	*temp1;
// 	char	*temp2;
// 	char	*new;

// 	temp1 = ft_strdup(str1);
// 	free(str1);
// 	temp2 = ft_strdup(str2);
// 	free(str2);
// 	new = ft_strjoin(temp1, temp2);
// 	free(temp1);
// 	free(temp2);
// 	return(new);
// }