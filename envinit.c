#include "minishell.h"

t_node	*env_create(char **env)
{
	int		i;
	char	**arr;
	t_node	*head;
	t_node	*node;

	i = 0;
	node = head;
	if (env[i] != 0)
	{
		env_init(env[i]);
		connect(&head, node);
		tail = node;
	}
	while (env[i] != 0)
	{
		env_init(env[i]);
		connect(&tail->next, node);
		tail = node;
	}
	tail->next = NULL;
	return (head);
}

void	connect(t_node **list, t_node *content)
{
	content->next = *list;
	*list = content;
}

t_node	*env_init(char *content)
{
	t_node	*tmp;
	char	**arr;

	array = ft_split(env[i], '=');
	tmp = malloc(sizeof(t_node));
	tmp->key = array[0];
	tmp->content = array[1];
	tmp->next = NULL;
	free(array);
	return (tmp);
}

