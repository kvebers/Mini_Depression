/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwillert <jwillert@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/20 14:16:43 by kvebers           #+#    #+#             */
/*   Updated: 2023/04/12 16:22:04 by jwillert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <stdlib.h>
# include "libme.h"	// needed for t_vector_str

int	g_signal;

enum e_outputs
{
	ERROR = -1,
	COMMAND_NOT_FOUND = -2,
	SYNTAX_ERROR = -3,
	PERMISSION_ERROR = -4,
	LOGIC_ERROR = -5,
	ENVIROMENTAL_ERROR = -6,
	PIPE_ERRORS = -7,
	VARIABLE_ERRORS = -8,
	EXECUTED = 0,
	ADD = 1,
	WHITE = 2,
	APOSTROPHE = 3,
	QUOTATION_MARK = 4,
	PIPE = 5,
	DOLLA = 6,
	COMMAND_TO_FILE = 7,
	FILE_TO_COMMAND = 8,
	EQUALS = 9,
	STRING = 10,
	SHELL_REDIRECTION = 17,
	HERE_DOC = 18,
	ECHO = 20,
	ECHO_N = 21,
	CD = 22,
	PWD = 23,
	EXPORT = 24,
	UNSET = 25,
	ENV = 26,
	EXIT = 27,
	N = 28,
	LAST_PIPE = 50,
	BUILTIN = 200,
	EXTERN = 100
};

typedef struct s_dump
{
	int	a;
	int	b;
	int	c;
	int	d;
}	t_dump;

typedef struct s_execute
{
	char	*order_str;
	char	*full_path;
	int		order_numb;
	int		number;
}	t_execute;

typedef struct s_combine
{
	t_execute	*command;
	char		*combined_str;
	char		*first_element;
	char		*full_path;
	int			count_n;
}	t_combine;

typedef struct s_heredoc
{
	struct s_heredoc	*next;
	char				*full_path;
	unsigned long int	hash;
	int					fd;
}	t_heredoc;

typedef struct s_data
{
	t_execute		*execute;
	t_combine		*combine;
	t_heredoc		*heredoc;
	char			**args;
	char			**envp;
	char			**argv;
	char			*line;
	int				*child_pids;
	int				commands_to_process;
	int				counter_pipes;
	int				counter_processes;
	int				index_processes;
	int				tokens;
	int				flag_heredoc;
	int				flag_infile;
	int				flag_outfile;
	int				fd_infile;
	int				fd_outfile;
}	t_data;

/* ************************************************************************** */
//                                     CORE
/* ************************************************************************** */

int		init_data(t_data *data);
//char	**free_tokens(char **tokens);
void	free_char_array(char **array_to_free);
void	free_loop(t_data *data);
int		argument_protection(t_data *data, int argc, char **argv, char **envp);
void	free_pipe_array(int **array, int size);
//int   utils_check_for_chars(t_data *data, int segment);

/* ************************************************************************** */
//                                    UTILS
/* ************************************************************************** */

char	*ft_strjoin2(char *temp, char *buffer, size_t cnt1, size_t cnt2);
int		is_white_space(char c);
int		is_pipe(int c);
int		ft_strnstr2(const char *haystack, const char *needle, size_t t);
int		utils_is_command_helper(t_data *data, int cnt1, int cnt);
int		utils_is_command_helper1(t_data *data, int cnt1, int cnt, char *needle);
int		is_command(t_data *data, int cnt, char *needle);
int		is_command1(t_data *data, int cnt, char *needle);
int		skip_white_spaces(t_data *data, int cnt);
int		dolla_handler(t_data *data, int cnt, int cnt1);
int		is_micro_pipe(int c);
int		count_split(char **split);
void	split_free(char **split);

/* ************************************************************************** */
//                                    LEXER
/* ************************************************************************** */

int		lexer(t_data *data);
char	**tokenizer(t_data *data, int cnt, int char_counter, int temp_char);
int		is_n(char *str);
int		remove_usless_quotes(t_data *data, int quote_state, int cnt);
int		remove_usless_quotes2(t_data *data, int quote_state, int cnt);

/* ************************************************************************** */
//                                    PARSER
/* ************************************************************************** */

int		parser(t_data *data);
int		parsing_error_handler(t_data *data);
int		check_if_combine_is_valid(t_data *data);
int		set_up_command_struct(t_data *data, int cnt, int cnt1, int switcher);
int		check_quote_state(t_data *data, int cnt);
int		strjoin_with_extra_steps(t_data *data, int cnt, int cnt1);
int		handle_quotes(t_data *data, int cnt, int cnt1);
void	init_t_combine(t_combine *combine, t_data *data);
int		i_hate_norm(t_data *data);
int		parser_count_strings(t_data *data, int cnt);
int		parser_return_q_m(t_data *data, int cnt, int cnt1);
int		parser_return_a(t_data *data, int cnt, int cnt1);
int		no_quote(t_data *data, int cnt, int cnt1);
int		find_main_command(t_data *data, int cnt, int cnt1, int switcher);
int		recheck_the_main_command(t_data *data, int cnt);
char	*ft_strnstr3(const char *haystack, const char *needle, size_t length);
int		token_error_handeler(t_data *data);
int		syntax_errors(t_data *data);
void	check_echo_n(t_data *data);
char	*search_needle(t_data *data, char *needle);
int		merge(t_data *data, int cnt, int cnt1);
int		retokenize_the_commands(t_data *data, int cnt);
int		retokenize_arrows(t_data *data);

/* ************************************************************************** */
//                                    REDIRECTOR
/* ************************************************************************** */

int		redirector(t_data *data);
void	redirector_handler_input(t_data *data);
void	redirector_handler_output(t_data *data);
int		redirector_prehandle_redirections(t_data *data, int counter_redirections);

/* ************************************************************************** */
//                                    EXECUTOR
/* ************************************************************************** */

int		executor(t_data *data);

/* ************************************************************************** */
//                                    BUILTINS
/* ************************************************************************** */

void	renumber_echo(int *numbered, char *str, int cnt);
void	num_echo(int *numbered, char *str, int quote_state, int cnt);
void	echo(t_combine str);
void	echo_n(t_data *data, int index);
int		is_builtin(int cmd_to_check);
char	builtin_pwd(void);
void	env(t_data *data);
int		echo_n_quote_state(char *str, int cnt, int quote_state);
char	*ft_charjoin(char *temp, char c, size_t cnt1, size_t cnt2);
char	*echo_n_helper(t_data *data, int cnt, char *comb);
char	*echo_merge(char *str, size_t cnt, int quote_state);

/* ************************************************************************** */
//                                    SIGNALS
/* ************************************************************************** */

void	handle_signal(int sig);

/* ************************************************************************** */
//                                    DEBUG
/* ************************************************************************** */

# ifndef DEBUG
#  define DEBUG 0
# endif  // DEBUG

void	debug_print_pid(char *process_name);
void	debug_print_char_array(char **args, char *name);
void	debug_print_int(char *description, int int_to_print);
void	debug_print_t_execute(t_data *data,	t_execute *execute);
void	debug_print_t_vector_str(t_vector_str *vector_to_print);
void	debug_print_t_combine(t_data *data);
void	debug_print_pipe_status(char *message, int **fd_pipes);
void	debug_fds(int max);

#endif  // MINISHELL_H
