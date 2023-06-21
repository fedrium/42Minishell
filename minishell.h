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

typedef	struct s_token
{
	char	*token;
	char	*expansion;
	int		priority;
}		t_token;


void	echo(t_list *line);
void	cd(char *line);
void	pwd(void);
void	pr_env(t_list	*head);
t_list *env_init(char **env);
t_env *new_env(char **splitted_env);
t_list	*tokenize(char *line);
t_token *get_token(char *line, int *p);
void export(t_list *head_env, t_list *head_tokens);
char *find_smallest_key(t_list *head, char *smallest_key);
int print_output(t_list *head, char *smallest_key);

#endif