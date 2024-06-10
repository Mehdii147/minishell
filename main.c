/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saharchi <saharchi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 16:13:31 by saharchi          #+#    #+#             */
/*   Updated: 2024/06/10 05:41:58 by saharchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int check(char c)
{
	if (c != ' ' && !(c >= 9 && c <= 13) && c != '"' && c != 39 && c != '<' && c != '>' && c != '|')
		return (0);
	return (1);
}
void parse_line(char *line, t_parse **parse)
{
	int i;
	int j;
	int index;
	
	
	index = 0;
	i = 0;
	while (line[i])
	{
		j = 0;
		while (line[i] == ' ' || (line[i] >= 9 && line[i] <= 13))
			i++;
		if (line[i] == '|' || line[i] == '<' || line[i] == '>')
		{
			if (line[i] == '|')
				ft_lstadd_back(parse, ft_lstnew("|", PIPE, index++));
			else if (line[i] == '<' && line[i + 1] == '<')
			{
				ft_lstadd_back(parse, ft_lstnew("<<", HDOC, index++));
				i++;
			}
			else if (line[i] == '<' && line[i + 1] != '<')
			{
				ft_lstadd_back(parse, ft_lstnew("<", RIN, index++));
			}
			else if (line[i] == '>' && line[i + 1] == '>')
			{
				ft_lstadd_back(parse, ft_lstnew(">>", APP, index++));
				i++;
			}
			else
				ft_lstadd_back(parse, ft_lstnew(">", ROUT, index++));
		}
		else if (line[i] == '"')
		{
			j = i;
			while (line[i])
			{
				if(line[i] == '\'' && line[i + 1] == ' ')
				{
					i++;
					break;
				}
				i++;	
			}
			ft_lstadd_back(parse, ft_lstnew(ft_substr(line, j, i), DQ, index++));
		}
		else if (line[i] == '\'')
		{
			j = i;
			while (line[i])
			{
				if(line[i] == '\'' && line[i + 1] == ' ')
				{
					i++;
					break;
				}
				i++;
			}
			ft_lstadd_back(parse, ft_lstnew(ft_substr(line, j, i), SQ, index++));
		}
		else
		{
			j = i;
			while(line[i] && !check(line[i]))
				i++;
			i--;
			ft_lstadd_back(parse, ft_lstnew(ft_substr(line, j, i+1-j), WORD, index++));
		}
		i++;
	}
}

int main(int ac, char **av, char **env)
{
	char *line;
	// char **en;
	t_parse *parse;
	t_parse *print;
	(void) env;
	(void) ac;
	(void) av;
	while (1)
	{
		line = readline("minishell :");
		if(!line)
			break;
		parse_line(line, &parse);
		
		print = parse;
		while(print)
		{
			printf("txt : %s\n", print->text);
			
			print = print->next;
		}
		print = NULL;
		parse = NULL;
		add_history(line);
		free(line);
	}
}


// ============ txt = | || token = PIPE ||  inde = 1
// ============ txt =  << || token = HDOC || inde = 2
// ============ txt =  << || token = HDOC || inde = 3
// ============ txt =  echo || token = WORD || inde = 4

// tttetttetestt 


