/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalee <yalee@student.42.fr.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 15:16:02 by yalee             #+#    #+#             */
/*   Updated: 2023/09/21 15:40:05 by yalee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env(t_list *env, char *line)
{
	t_env	*temp;

	while (env->next != NULL)
	{
		temp = (t_env *)env->content;
		if (ft_strncmp(temp->key, line, ft_strlen(line) + 1) == 0)
			return (temp->value);
		env = env->next;
	}
	return (0);
}

int	cd_stay_on_current_dir(t_list *env)
{
	chdir(get_env(env, "HOME"));
	return (1);
}

void	cd_update_env(t_list *env, char *old, char *now)
{
	t_env	*temp;

	while (env != NULL)
	{
		temp = (t_env *)env->content;
		if (ft_strncmp("PWD", temp->key, 3) == 0)
		{
			free(temp->value);
			temp->value = ft_strdup(now);
		}
		if (ft_strncmp("OLDPWD", temp->key, 6) == 0)
		{
			free(temp->value);
			temp->value = ft_strdup(old);
		}
		env = env->next;
	}
}

void	cd(t_list *env, t_list *token, int size)
{
	char	*now;
	char	*line;
	char	*old;

	if (size == 1 && cd_stay_on_current_dir(env))
		return ;
	line = ((t_token *)token->next->content)->token;
	old = getcwd(NULL, 1024);
	if (line[0] == '~')
		chdir(get_env(env, "HOME"));
	else if (chdir(line) == -1)
		printf("bash: cd: %s: No such file or directory\n", line);
	now = getcwd(NULL, 1024);
	cd_update_env(env, old, now);
	free(old);
	free(now);
}
