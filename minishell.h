#ifndef MINISHELL
# define MINISHELL

#include "libft/libft.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

typedef struct s_env
{
	char			*key;
	char			*value;
}				t_env;

void	echo(char *line);
void	cd(char *line);
void	pwd(void);
void	pr_env(t_list	*head);
t_list *env_init(char **env);
void export(t_list *head);
char *find_smallest_key(t_list *head, char *smallest_key);

#endif