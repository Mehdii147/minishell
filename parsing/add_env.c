/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehafiane <ehafiane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 15:45:45 by saharchi          #+#    #+#             */
/*   Updated: 2024/09/25 18:55:29 by ehafiane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	count_args(t_parse *parse)
{
	int	count;

	count = 0;
	while (parse && parse->token != PIPE)
	{
		if (parse->token == WORD)
		{
			if (parse->flag == 1 || parse->flag == 2)
				count += count_str(parse->text, " \t\n\v\f\r", parse->flag);
			else
				count++;
		}
		else if (parse->token == HDOC || parse->token == ROUT
			|| parse->token == APP || parse->token == RIN)
			parse = parse->next;
		parse = parse->next;
	}
	return (count);
}

t_env	*ft_envnew(char *key, char *value)
{
	t_env	*env;

	env = malloc(sizeof(t_env));
	if (!env)
		exit(1);
	env->key = key;
	env->value = value;
	env->next = NULL;
	return (env);
}

void	add_env(t_env **envs, char *key, char *value)
{
	t_env	*tmp;
	t_env	*new;

	new = ft_envnew(key, value);
	if (!new)
		return ;
	if (!*envs)
	{
		*envs = new;
		return ;
	}
	tmp = *envs;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

int	check_exits(t_env **envs)
{
	t_env	*tmp;

	tmp = *envs;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, "?") == 0)
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

void	ft_env(t_env **envs, char **env, int flag)
{
	int		i;
	char	*key;
	char	*value;

	(1) && (i = 0, key = NULL, value = NULL);
	while (env[i])
	{
		key = ft_substr(env[i], 0, ft_strchr(env[i], '=') - env[i]);
		value = ft_strdup(ft_strchr(env[i], '=') + 1);
		add_env(envs, key, value);
		i++;
	}
	if (flag == 1)
	{
		help_ft_env(envs);
	}
	if (!check_exits(envs))
		add_env(envs, ft_strdup("?"), ft_strdup("0"));
	else if (check_exits(envs))
		exit_status(envs, "0");
}
