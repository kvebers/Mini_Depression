/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pipex.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwillert <jwillert@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 16:47:19 by jwillert          #+#    #+#             */
/*   Updated: 2023/04/30 15:43:37 by jwillert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"	// needed for t_data, MACROS
#include "executor.h"	// needed for utils_is
#include <unistd.h>		// needed for pipe()

#include <stdio.h>

void	executor_parent_close_pipes(t_data *data, int **fd_pipes);

int	pipex_advance_to_next_pipe(t_data *data, int index)
{
	index += 1;
	while (index < data->commands_to_process)
	{
		if (data->combine[index].combined_str == NULL)
		{
			break ;
		}
		else if (data->combine[index].command->order_numb == PIPE
			|| data->combine[index].command->order_numb == LAST_PIPE)
		{
			data->index_processes += 1;
			index += 1;
			break ;
		}
		index += 1;
	}
	return (index);
}

int	executor_add_trailing_command(t_data *data, int index)
{
	int	offset;

	offset = index + 1;
	while (offset < data->commands_to_process
		&& data->combine[offset].combined_str != NULL
		&& data->combine[offset].command->order_numb != PIPE
		&& data->combine[offset].command->order_numb != LAST_PIPE)
	{
		if (data->combine[offset].command->order_numb == STRING
			|| is_builtin(data->combine[offset].command->order_numb) == 1
			|| data->combine[offset].command->order_numb == N
			|| data->combine[offset].command->order_numb == WIERD_N)
		{
			data->combine[offset].command->order_numb = WHITE;
			data->combine[index].combined_str
				= ft_charjoin(data->combine[index].combined_str, ' ', 0, 0);
			if (data->combine[index].combined_str == NULL)
			{
				return (ERROR);
			}
			data->combine[index].combined_str
				= ft_strjoin2(data->combine[index].combined_str,
					data->combine[offset].combined_str, 0, 0);
			if (data->combine[index].combined_str == NULL)
			{
				return (ERROR);
			}
		}
		offset += 1;
	}
	return (EXECUTED);
}

int	pipex_skip_non_commands(t_data *data, int index)
{
	while ((index < data->commands_to_process
			&& data->combine[index].combined_str != NULL
			&& data->combine[index].command->order_numb != STRING
			&& is_builtin(data->combine[index].command->order_numb) == 0
			&& data->combine[index].command->order_numb != PIPE
			&& data->combine[index].command->order_numb != LAST_PIPE))
	{
		index += 1;
	}
	return (index);
}

static int	**pipex_create_pipes(int counter_pipes)
{
	int	**fd_pipes;
	int	index;

	index = 0;
	debug_print_stage("pipex", 1);
	fd_pipes = malloc (sizeof (int *) * (counter_pipes + 1));
	if (fd_pipes == NULL)
	{
		return (NULL);
	}
	while (index < counter_pipes)
	{
		fd_pipes[index] = malloc (sizeof (int) * 2);
		if (fd_pipes[index] == NULL
			|| pipe(fd_pipes[index]) == ERROR)
		{
			free_pipe_array(fd_pipes, index);
			return (NULL);
		}
		index += 1;
	}
	fd_pipes[counter_pipes] = NULL;
	return (fd_pipes);
}

int	executor_pipex(t_data *data)
{
	int	**fd_pipes;
	int	index;

	index = 0;
	fd_pipes = pipex_create_pipes(data->counter_pipes);
	if (fd_pipes == NULL)
	{
		return (ERROR);
	}
	while (index < data->commands_to_process)
	{
		debug_print_pipe_status(data, "parent before execution", fd_pipes);
		data->exit_status = 0;
		if (executor_cmd_selector(data, fd_pipes, index) == ERROR)
		{
			free_pipe_array(fd_pipes, data->counter_pipes);
			return (ERROR);
		}
		index = pipex_advance_to_next_pipe(data, index);
		data->fd_infile = -1;
		data->fd_outfile = -1;
		data->flag_infile = 0;
		data->flag_outfile = 0;
	}
	if (fd_pipes != NULL && data->counter_pipes != 0)
	{
		executor_parent_close_pipes(data, fd_pipes);
	}
	free_pipe_array(fd_pipes, data->counter_pipes);
	return (EXECUTED);
}
