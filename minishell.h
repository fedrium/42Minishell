#ifndef MINISHELL
# define MINISHELL

#include "libft/libft.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <termios.h>
#include <dirent.h>
#include <sys/types.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/wait.h>

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
}			t_token;

typedef	struct s_lexrvar
{
	int		in_squote;
	int		in_dquote;
	int		i;
	int		cacoon_pos;
	int		*targets;
	char	*meta_c;
}			t_lexrvar;

void	echo(t_list *line);
void cd(t_list *env, t_list *token, int size);
void pwd(void);
void	pr_env(t_list	*head);
t_list *env_init(char **env);
t_env *new_env(char **splitted_env);
t_list	*tokenize(char *line, t_list *head_env);
t_token *get_token(char *line, int *p);
void export(t_list *head_env, t_list *head_tokens);
char *find_smallest_key(t_list *head, char *smallest_key);
int print_output(t_list *head, char *smallest_key);
void	cleanse(t_list *node, t_list *head_env);
int	check_invalid(t_list *head_tokens, int mute);
void	expand_n_join(char **temp, char **new, int *i,t_list *head_env);
int	can_move(char c, int *i, int *isq, int *idq);
void	exiting(t_list *token, int size);
void	unset(t_list **env, t_list *head_token, int size);
void	tlistmover(t_list **env);
void	signal_handler(int signal);
void	sig(void);
void	setatt();
char	**env_arr(t_list *env);
void	exe(t_list *env, char *cmd, char *path);
char	*getvalue(t_list *env, char *key);
int		get_file(char *cmd, t_list *env);
char	**path_format(t_list *env);
void	freeing(char **array);


#endif