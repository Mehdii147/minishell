/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saharchi <saharchi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 15:41:50 by saharchi          #+#    #+#             */
/*   Updated: 2024/08/29 05:02:57 by saharchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	print_error(char *text)
{
	ft_putstr_fd("Minishell: syntax error near unexpected token `", 2);
	ft_putstr_fd(text, 2);
	ft_putstr_fd("'\n", 2);
}

int	ch_fexp(char c, int i)
{
	if (i == 0 && ((ft_isdigit(c) || ft_isalpha(c) || c == '_' || c == '?')))
		return (1);
	else if (i == 1 && (ft_isdigit(c) || ft_isalpha(c)
			|| c == '_' || c == '?' || c == '"' || c == '\''))
		return (1);
	return (0);
}

int	count_forexp(char *str, int j)
{
	while (str[j] && ch_fexp(str[j], 0))
	{
		if (str[j] == '?')
		{
			j++;
			break ;
		}
		j++;
	}
	return (j);
}

char	*add_quot(char *value, char quote)
{
	char	*s;

	if (ft_strchr(value, '\'') && quote != '"')
	{
		s = value;
		value = ft_strjoin(ft_strdup("\""), s);
		free(s);
		value = ft_strjoin(value, "\"");
	}
	else if (ft_strchr(value, '"') && quote != '"')
	{
		s = value;
		value = ft_strjoin(ft_strdup("\'"), s);
		free(s);
		value = ft_strjoin(value, "\'");
	}
	else if (ft_strchr(value, '"') && quote == '"')
	{
		s = value;
		value = ft_strjoin(ft_strdup("\"\'"), s);
		free(s);
		value = ft_strjoin(value, "\'\"");
	}
	return (value);
}

char	*return_value(char *str, int i, t_env *envs, t_flag *flag)
{
	int		j;
	char	*new;
	char	*strtmp;
	char	*value;

	j = count_forexp(str, i + 1);
	new = ft_substr(str, 0, i);
	value = check_value(ft_substr(str, i + 1, j - i - 1), envs, flag->flag);
	if (value)
		value = add_quot(value, flag->quote);
	strtmp = ft_strjoin(new, value);
	if (value[0] != '\0')
		free(value);
	if (!value)
		free(new);
	new = ft_substr(str, j, ft_strlen(str) - j);
	strtmp = ft_strjoin(strtmp, new);
	free(new);
	free(str);
	return (strtmp);
}

char	*expand_str(char *str, t_env *envs, t_flag *flag)
{
	int		i;
	int		j;
	char	quote;

	(1) && (i = 0, j = 0, quote = '\0');
	while (str[i])
	{
		j++;
		if (str[i] != '$')
			j = 0;
		if (quote == '\0' && (str[i] == '\'' || str[i] == '"'))
			quote = str[i];
		else if (quote == str[i] && (str[i] == '\'' || str[i] == '"'))
			quote = '\0';
		if (str[i] == '$' && (j % 2 != 0)
			&& ((quote != '\'' && ch_fexp(str[i + 1], 0))
				|| (quote == '\0' && ch_fexp(str[i + 1], 1))))
			(1) && (str = return_value(str, i, envs, flag), i--);
		if (ft_strcmp(str, "") == 0)
			return (str);
		i++;
	}
	return (str);
}

char	is_quote(char *text)
{
	int		j;
	char	quote;

	j = 0;
	quote = '\0';
	while (text[j] && text[j] != '$')
	{
		if (quote == '\0' && text[j] == '"')
			quote = text[j];
		else if (text[j] == quote)
			quote = '\0';
		j++;
	}
	return (quote);
}

int	count_quotes(char *text)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (text[i] && text[i] != '$')
	{
		if (text[i] == '"')
			j++;
		if (j && text[i] != '"')
			break ;
		i++;
	}
	return (j);
}

int	check_fsplit(char *str)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == '$')
			j = 1;
		if (str[i] == '=' && j == 1)
			return (1);
		i++;
	}
	return (0);
}

void	git_expand(t_flag flag, t_parse *tmp, char *str, t_env *envs)
{
	int		j;
	int		i;

	flag.quote = is_quote(tmp->text);
	i = check_fsplit(tmp->text);
	j = count_quotes(tmp->text);
	tmp->text = expand_str(tmp->text, envs, &flag);
	if ((is_space(tmp->text) && ft_strcmp(str, "export") && (j % 2 == 0))
		|| ft_strcmp(tmp->text, "") == 0)
		tmp->flag = 2;
	else if ((is_space(tmp->text) && !ft_strcmp(str, "export")) && i == 1)
		tmp->flag = 1;
}

void	ft_expand(t_parse **parse, t_env *envs)
{
	t_flag	flag;
	t_parse	*tmp;
	char	*str;

	(1) && (flag.flag = 0, tmp = *parse, str = ft_strdup(""));
	while (tmp)
	{
		if (tmp->token == WORD && ft_strcmp(tmp->text, "export") == 0
			&& tmp->next)
		{
			free(str);
			str = ft_strdup(tmp->text);
		}
		if (tmp->token == HDOC || tmp->token == RIN
			|| tmp->token == APP || tmp->token == ROUT)
			tmp = tmp->next;
		else if (tmp->token == WORD && ft_strchr(tmp->text, '$'))
			git_expand(flag, tmp, str, envs);
		if (tmp->token == PIPE)
			flag.flag = 1;
		free(str);
		str = ft_strdup(tmp->text);
		tmp = tmp->next;
	}
	free(str);
}
