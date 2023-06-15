#ifndef MINISHELL
# define MINISHELL

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "libft/libft.h"

typedef struct s_env
{
	char			*key;
	char			*value;
}				t_env;

typedef	struct s_variables
{
	int	temp1;
	int	temp2;
	int	temp3;
};

void	echo(char *line);
void	cd(char *line);
void	pwd(void);
void	pr_env(t_list	*head);
t_list *env_init(char **env);

#endif