/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saharchi <saharchi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 16:13:38 by saharchi          #+#    #+#             */
/*   Updated: 2024/07/15 03:29:13 by saharchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include <readline/readline.h>
#include <readline/history.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <string.h>
# include <fcntl.h>
# include <dirent.h>
# include <sys/wait.h>
# include <limits.h>
# include <errno.h>
# include <signal.h>
#include "./libft/libft.h"
#include <fcntl.h>

typedef enum s_token
{
	WORD, // simple word
	HDOC, // << heredoc
	RIN, // < rediction in
	APP, // append >> 
	ROUT, // redirection out >
	PIPE, // |
}	t_token;


 typedef struct s_fd
{
	int fd_in;
	int fd_out;
} t_fd;
typedef struct s_cmd
{
	char **args;
	t_fd fd;
	struct s_cmd *next;
} t_cmd;


typedef struct s_env
{
	char *key;
	char *value;
	struct s_env *next;
} t_env;

typedef struct s_parse
{
	char *text; 
	t_token token;
	struct s_parse *next;
} t_parse;

typedef struct s_data
{
	char *line;
	struct s_cmd *cmd;
	struct s_env *env;
} t_data;

t_parse	*ft_lstnew(char *content, t_token token);
void	ft_lstadd_back(t_parse **lst, t_parse *new);
t_parse	*ft_lstlast(t_parse *lst);
int	ft_lstsize(t_parse *lst);
void	ft_lstclear(t_parse *lst);

#endif