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

void	ll_free(t_list *ll)
{
	t_env	*ll_temp;

	while (ll)
	{
		ll_temp = (t_env *)ll;
		free(ll_temp->key);
		free(ll_temp->value);
		ll = ll->next;
	}
}