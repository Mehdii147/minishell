/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_status.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saharchi <saharchi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 06:57:44 by saharchi          #+#    #+#             */
/*   Updated: 2024/08/21 14:11:10 by saharchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	exit_status(t_env **env, char *status)
{
	t_env	*tmp;

	if (!env || !*env)
		return ;
	tmp = *env;
	while (tmp)
	{
		if (!ft_strcmp(tmp->key, "?"))
		{
			free(tmp->value);
			tmp->value = ft_strdup(status);
			return ;
		}
		tmp = tmp->next;
	}
}

void	ft_free(char **str)
{
	int	i;

	i = 0;
	while (str[i])
		free(str[i++]);
	free(str);
}

int	handle_expand(t_env *env, char *str, int token)
{
	int		fd;
	char	*tmp;
	t_flag	flag;

	flag.flag1 = 0;
	flag.flag = 0;
	flag.quote = '\0';
	fd = 0;
	tmp = expand_str(ft_strdup(str), env, &flag);
	if (ft_strcmp(tmp, "") == 0)
		fd = -2;
	if (is_space(tmp))
	{
		tmp = ft_strtrim(tmp, " \t\n\v\f\r");
		if (is_space(tmp) || ft_strcmp(tmp, "") == 0)
			fd = -2;
	}
	if (fd != -2)
	{
		if (token == ROUT)
			fd = open(tmp, O_CREAT | O_RDWR | O_TRUNC, 0664);
		else if (token == APP)
			fd = open(tmp, O_CREAT | O_RDWR | O_APPEND, 0664);
		else
			fd = open(tmp, O_RDONLY, 0644);
	}
	free(tmp);
	return (fd);
}

int	ha_re_in(char *file, t_env *env, int token)
{
	int	fd;

	if (ft_strchr(file, '$'))
	{
		fd = handle_expand(env, file, token);
		return (fd);
	}
	if (!ft_strcmp(file, ""))
		return (-2);
	fd = open(file, O_RDONLY, 0644);
	return (fd);
}

int	ha_re_ou(char *file, t_env *env, int token)
{
	int	fd;

	if (ft_strchr(file, '$'))
	{
		fd = handle_expand(env, file, token);
		return (fd);
	}
	if (token == ROUT)
		fd = open(file, O_CREAT | O_RDWR | O_TRUNC, 0664);
	else
		fd = open(file, O_CREAT | O_RDWR | O_APPEND, 0664);
	return (fd);
}