/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalee <yalee@student.42.fr.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 16:19:10 by yalee             #+#    #+#             */
/*   Updated: 2023/09/28 21:57:23 by yalee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <signal.h>
# include <termios.h>
# include <dirent.h>
# include <sys/types.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>
# include <fcntl.h>

// global variable for error code
int		g_ercode;

// structs
// struct for enviromental variables linked list
typedef struct s_env
{
	char			*key;
	char			*value;
}				t_env;

// struct for main
typedef struct s_main_vars
{
	int				out;
	char			*line;
	int				in;
	t_list			*head_env;
	t_list			*head_tokens;
	struct termios	saved;
}			t_main_vars;

// struct for execve
typedef struct s_execve_vars
{
	int				i;
	DIR				*cur_dir;
	char			**path;
	char			**cmd_arr;
	char			*exec_cmd;
	pid_t			pid;
	struct dirent	*cur_file;
}			t_execve_vars;

// lexer structs
// struct for lexer tokens linked list
typedef struct s_token
{
	char			*token;
	int				priority;
	struct s_token	*next;
}			t_token;

// struct for data cleaning
typedef struct s_lexer_cleanse_vars
{
	char	*temp;
	char	*new;
	char	join[2];
	int		i;
	int		in_squote;
	int		in_dquote;
}		t_cleanse_vars;

// piping structs
// struct for child process
typedef struct s_cp
{
	t_list		*tokens;
	int			pipe[2];
	struct s_cp	*next;
}		t_cp;

// struct for keeping track of fd and child processes
typedef struct s_piping_vars
{
	t_cp	*child_processes;
	t_cp	*cp_head;
	pid_t	pid;
	int		ori_stdout;
	int		ori_stdin;
	int		num_processes;
}		t_piping_vars;

// signal handler
void			sig(t_list *head_tokens, t_list *head_env);
void			signal_handler(int num);
void			signulll_exit(t_main_vars *main_vars);
void			setatt(void);

// lexer
void			init_get_token(int *quote, int *squote,
					t_token **token, char (*join)[2]);
t_cleanse_vars	*init_cleanse(t_list *node);
t_list			*tokenize(char *line, t_list *head_env);
t_token			*get_token(char **line, int *p, t_list *env);
void			cleanse(t_list *node, t_list *head_env);
char			*lexer_strjoin(char *str1, char *str2);
void			expand_n_join(char **line, int *p, int quote, t_list *env);
void			expansion_get_value(t_list **tenv, char **key, char **value);
char			*exp_get_key(char *line, int p, int quote);
char			*exp_get_value(char *key, t_list *env);
void			check_quotes(t_cleanse_vars *cleanse_vars, t_list *node);
int				check_invalid(t_list *head_tokens, int mute);
void			check_head_tokens(t_list *node, char *line);
int				is_last_arg(t_list *head_tokens);
char			is_special(t_list *node);
int				can_move(char c, int *i, int *isq, int *idq);
char			*get_tl_str(t_list *node);
void			copy_after_meta(char **line, int *p, char *value, int *after);
void			copy_b4_meta(char **line, int *p);

// pwd
void			pwd(void);
// cd
void			cd(t_list *env, t_list *token, int size);
int				cd_stay_on_current_dir(t_list *env);
char			*get_env(t_list *env, char *line);
void			cd_update_env(t_list *env, char *old, char *now);
// echo
void			echo(t_list *line);
// unset
void			init_unset(t_list **head_env, t_list **prev_env,
					t_list **slave, t_list **env);
void			unset(t_list **env, t_list *token, int size);
// exit
void			exit_func(t_list *head_tokens, t_list *head_env,
					t_main_vars *main_vars);
// env
void			pr_env(t_list *head);
// export
void			export(t_list **head_env, t_list *head_tokens);
int				export_syntax_check(t_list *node);
void			add_to_envlst(t_list **head_env, t_list *node, t_env *env);
void			add_env(t_list **head_env, t_list *head_tokens);
char			*find_smallest_key(t_list *head, char *smallest_key);
int				print_output(t_list *head, char *smallest_key);
// execve
t_execve_vars	*init_get_file(t_list *head_tokens, t_list *env);
char			*getvalue(t_list *env, char *key);
char			**path_format(t_list *env);
char			**env_arr(t_list *env);
char			**convert_list(t_list *head_tokens);
int				get_file_abs_path(t_execve_vars *execve_vars, t_list *env);
int				get_file_normal(t_execve_vars *execve_vars, t_list *env);
void			get_file(t_list *head_tokens, t_list *env);
int				get_file_abs_path_nopp(t_execve_vars *execve_vars, t_list *env);
int				get_file_normal_nopp(t_execve_vars *execve_vars, t_list *env);
void			get_file_nopp(t_list *head_tokens, t_list *env);
char			*strjoin_helper(char *path, char *cmd);

// enviromental variable simulator
t_list			*env_init(char **env);
t_env			*new_env(char **splitted_env);

// executor (piping, redirection, heredoc)
t_main_vars		*init_basic(char **env);
void			executor(t_main_vars *main_vars);
// piping
t_piping_vars	*init_pipes(t_list *head_tokens);
void			split_args(t_list **segment, t_list **head_tokens);
void			organise_args(t_list *head_tokens, t_list **head_env,
					t_main_vars *main_vars);
t_cp			*make_cp(t_list *head_tokens, int *out, int *in);
void			run_functions(t_list *head_tokens, t_list **head_env,
					t_main_vars *main_vars);
void			run_functions_nopp(t_list *head_tokens, t_list **head_env,
					t_main_vars *main_vars);
void			start_cp(t_piping_vars *piping_vars, t_list **head_env,
					t_main_vars *main_vars);
void			wait_cp(t_piping_vars *piping_vars);
int				get_cp_size(t_cp *cp);
void			iterate_list(t_piping_vars **piping_vars);
// redirection and heredoc
void			ll_iter(t_list *head_tokens);
int				redir_check(t_list *head_tokens);
void			find_and_run_process(t_list **head, t_list **start,
					int *fd, int *redir);
int				redir_app(t_list *head, t_list *start, int fd);
int				redir_rep(t_list *head, t_list *start, int fd);
int				redir_in(t_list *head, t_list *start, int fd);
void			heredoc(char *delimeter);
int				redir_heredoc(t_list *head, t_list *start, int fd);
void			cntl_c(int num);

// garbage collectors
void			free_unset(t_list *temp_env);
void			free_getfile_norm(t_execve_vars *execve_vars);
void			redir_rm(t_list **head, t_list *to_del);
void			lst_free_env(t_list *head_env);
void			lst_free_one(t_list *node);
void			lst_free_all(t_list *head);
void			free_2dar(char **ar_2d);
void			free_cp(t_cp *head);
void			free_execve(t_execve_vars *execve_vars);

#endif
