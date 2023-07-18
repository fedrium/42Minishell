#include "minishell.h"

char	**env_arr(t_list *env)
{
	int		size;
	int		i;
	char	*str;
	char	**array;
	t_env	*temp;

	size = ft_lstsize(env);
	array = malloc(sizeof(char *) * size);
	i = 0;
	while (env->next != NULL)
	{
		temp = (t_env *)env->content;
		str = ft_strjoin(temp->key, "=");
		array[i] = ft_strjoin(str, temp->value);
		env = env->next;
	}
	return array;
}

void	extra_bi(t_list *env, t_list *head_tokens)
{
	
}