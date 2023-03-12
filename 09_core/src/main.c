/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvebers <kvebers@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/20 14:13:47 by kvebers           #+#    #+#             */
/*   Updated: 2023/03/12 21:30:32 by kvebers          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/history.h>
#include "libft.h"

static int	history(t_data *data)
{
	data->line = readline("Terminal Troublemakers: ");
	if (data->line == NULL)
		return (ERROR);
	else if (data->line)
	{
		add_history(data->line);
		if (lexer(data) == ERROR)
			return (free(data->line), ERROR);
	}
	return (EXECUTED);
}

int	main(int argc, char **argv, char **envp)
{
	t_data	data;

	using_history();
	if (argument_protection(&data, argc, argv, envp) == ERROR)
		return (ERROR);
	debug_print_t_expander(data.expander);
	while (1)
	{
		if (history(&data) == ERROR)
			break ;
		if (parser(&data) == ERROR)
		{
		}
		// if expander
		// if execution	
		free_loop(&data);
	}
	return (EXECUTED);
}
