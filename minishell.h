#ifndef MINISHELL
# define MINISHELL

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

typedef struct s_node
{
	char			*key;
	char			*content;
	struct s_node 	*next;
}				t_node;

void	echo(char *line);
void	ft_putstr_fd(char *s, int fd);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
int		ft_strlen(const char *str);
char	**ft_split(char const *s, char c);
void	cd(char *line);
void	pwd(void);
void	pr_env(char **env);

#endif