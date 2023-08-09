#include "minishell.h"

char    *get_tl_str(t_list *node)
{
    return(((t_token *)node->content)->token);
}