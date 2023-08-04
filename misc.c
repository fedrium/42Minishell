#include "minishell.h"

void	freeing(char **array)
{
	int	i;

	i = 0;
	while (array[i] != 0)
	{
		free(array[i]);
		i++;
	}
	free(array);
}

void	ll_free(t_list **ll)
{
	// t_env	*ll_temp;
	t_list	*head;
	// t_list	*envt;
	int i = 0;

	head = *ll;
	// envt = NULL;
	while (head != NULL)
	{
		// printf("i: %i\n", i);
		// printf("%s\n", ((t_env *)head->content)->key);
		// if (((t_env *)head->content)->key)
			free(((t_env *)head->content)->key);
		// if (((t_env *)head->content)->value)
			free(((t_env *)head->content)->value);
		// envt->next = head->next;
		// envt = head;
		if (head != NULL)
			head = head->next;
		i++;
	}
}

char	*str(char *str)
{
	char	*temp;

	temp = ft_strdup(str);
	free(str);
	return (temp);
}