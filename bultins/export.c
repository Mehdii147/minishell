/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehafiane <ehafiane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 09:13:33 by ehafiane          #+#    #+#             */
/*   Updated: 2024/09/27 00:46:55 by ehafiane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	update_env(t_env **env, char *key, char *value)
{
	t_env	*current;

	current = *env;
	while (current)
	{
		if ((ft_strcmp(current->key, key) == 0) && value)
		{
			free(current->value);
			current->value = value;
			break ;
		}
		current = current->next;
	}
	free(key);
}

void	add_or_update_env(t_env **env, char *key, char *value)
{
	if (env_key_exists(*env, key))
		update_env(env, key, value);
	else
		add_env(env, key, value);
}

void	join_env(t_env *env, char *key, char *value)
{
	t_env	*current;

	current = env;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			current->value = ft_strjoin(current->value, value);
			free(value);
			free(key);
		}
		current = current->next;
	}
}

void	process_key_value(char *cmd, t_env **env)
{
	char	*key;
	char	*value;

	if (is_valid_key(cmd) == 2)
	{
		key = ft_substr(cmd, 0, ft_strchr(cmd, '+') - cmd);
		value = ft_strdup(ft_strchr(cmd, '+') + 2);
		if (env_key_exists(*env, key))
			join_env(*env, key, value);
		else
			add_or_update_env(env, key, value);
	}
	else if (ft_strchr(cmd, '='))
	{
		key = ft_substr(cmd, 0, ft_strchr(cmd, '=') - cmd);
		value = ft_strdup(ft_strchr(cmd, '=') + 1);
		add_or_update_env(env, key, value);
	}
	else
	{
		key = ft_strdup(cmd);
		add_or_update_env(env, key, NULL);
	}
}

int	ft_export(char **cmd, t_env **env, t_data *data)
{
	int		i;
	int		flag;

	flag = 0;
	i = 1;
	if (!cmd[i])
		return (sort_env(env, data), 0);
	while (cmd[i])
	{
		if (!is_valid_key(cmd[i]))
		{
			env_key_error(cmd, env, i, "export");
			exit_status(env, "1");
			flag = 1;
			i++;
			continue ;
		}
		process_key_value(cmd[i], env);
		i++;
	}
	if (!flag)
		exit_status(env, "0");
	return (0);
}
