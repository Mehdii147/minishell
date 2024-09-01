/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehafiane <ehafiane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 23:44:59 by ehafiane          #+#    #+#             */
/*   Updated: 2024/08/18 16:23:39 by ehafiane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	count_commands(t_cmd *cmd_list)
{
	int	num_cmds;

	num_cmds = 0;
	while (cmd_list)
	{
		num_cmds++;
		cmd_list = cmd_list->next;
	}
	return (num_cmds);
}

void	child_process(t_data *data, t_cmd *cmd_list, int *fd, int flag)
{
	dup2(data->temp, 0);
	if (cmd_list->next)
	{
		dup2(fd[1], 1);
		close_fd(fd);
	}
	handle_redirection(cmd_list);
	data->path = get_path(cmd_list->args[0], data->env);
	if (cmd_list->args[0])
	{
		exec_process(data, cmd_list, flag);
	}
}

void	parent_proccess(t_data *data, t_cmd *cmd_list, int pid)
{
	data->created_child = 1;
	if ((unsigned long)(data->cmd_index * 4) < sizeof(data->childpids))
		data->childpids[data->cmd_index++] = pid;
	if (data->temp != 0)
		close(data->temp);
	if (cmd_list->next)
		close(data->fd[1]);
	data->temp = data->fd[0];
}

void	execute_command(t_data *data, t_cmd *cmd_list)
{
	pid_t	pid;

	if (cmd_list->next)
		pipe_error(data, data->fd, &data->flag_exec);
	if (if_bultins(cmd_list->args) && data->flag_exec == 0
		&& cmd_list->flag == 0)
		one_bultin(data, cmd_list);
	else if (cmd_list->args[0] && cmd_list->flag == 0)
	{
		pid = fork();
		if (pid < 0)
		{
			failed_fork(data, data->fd);
			return ;
		}
		if (pid == 0)
			child_process(data, cmd_list, data->fd, data->flag_exec);
		else
			parent_proccess(data, cmd_list, pid);
	}
}

void	execute_this(t_data *data)
{
	t_cmd	*cmd_list;

	data->cmd_index = 0;
	data->created_child = 0;
	data->flag_exec = 0;
	data->temp = 0;
	cmd_list = data->cmd;
	while (cmd_list)
	{
		execute_command(data, cmd_list);
		cmd_list = cmd_list->next;
	}
	if (data->temp != 0)
		close(data->temp);
	if (data->created_child)
	{
		g_sigl.sig_child = 1;
		wait_pid_fun(data->cmd_index, data->childpids, data);
		g_sigl.sig_child = 0;
	}
}

// void execute_this(t_data *data)
// {
//  pid_t pid;
//  int fd[2];
//  int cmd_index = 0;
//  int num_cmds = 0;
//  int childpids[256];
//  int created_child = 0;
//  int flag = 0;
//  data->temp = 0;

//  t_cmd *cmd_list = data->cmd;
//  num_cmds = count_commands(cmd_list);
//  while (cmd_list)
//  {
//      if (cmd_list->next)
//          pipe_error(data , fd , &flag);
//      if (if_bultins(cmd_list->args) && flag == 0 && cmd_list->flag == 0)
//          one_bultin(data, cmd_list);
//      else if (cmd_list->args[0] && cmd_list->flag == 0)
//      {
//          pid = fork();
//          if (pid < 0)
//          {
//              failed_fork(data, fd);
//              break ;
//          }
//          if (pid == 0)
//              child_process(data, cmd_list, fd, flag);
//          else
//          {
//              created_child = 1;
//              if((unsigned long)(cmd_index * 4) < sizeof(childpids))
//                  childpids[cmd_index++] = pid;
//              if (data->temp  != 0)
//                  close(data->temp );

//              if (cmd_list->next)
//                  close(fd[1]);
//              data->temp  = fd[0];
//          }
//      }
//      cmd_list = cmd_list->next;
//  }
//  if (data->temp  != 0)
//      close(data->temp );
//  if (created_child)
//  {
//      g_sigl.sig_child = 1;
//      wait_pid_fun(cmd_index, childpids, data);
//      g_sigl.sig_child = 0;
//  }
// }
