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
t_env *new_env(char **splitted_env);
t_list	*tokenize(char *line);
char *get_token(char *line, int *p);
void missing_quote(char **token, char quote);
void export(t_list *head_env, t_list *head_tokens);
char *find_smallest_key(t_list *head, char *smallest_key);
int quote_in_line(char *line, char quote);
int print_output(t_list *head, char *smallest_key);
int is_last(char *line, int *p);
char *ft_joinchar(char *token, char c);
int is_quote(char c, char quote);
char *count_quotes(char **token, char q);
void remove_quotes(char **token, char q);
#endif