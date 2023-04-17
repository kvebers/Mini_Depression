# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jwillert <jwillert@student.42heilbronn.    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/03 12:52:07 by jwillert          #+#    #+#              #
#    Updated: 2023/04/18 00:36:27 by jwillert         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#	Programm Name
NAME							=	minishell

#	Directories
LIBALLME_DIR					=	./00_liballme/
GNL_DIR							=	$(LIBALLME_DIR)get_next_line
LIBFT_DIR						=	$(LIBALLME_DIR)libft/
LIBME_DIR						=	$(LIBALLME_DIR)libme/
HEADER_DIR						=	./01_includes/
LEXER_DIR						=	./02_lexer/
PARSER_DIR						=	./03_parser/
EXPANDER_DIR					=	./04_expander/
REDIRECTOR_DIR					=	./05_redirector/
EXECUTOR_DIR					=	./06_executor/
BUILTINS_DIR					=	./07_builtins/
SIGNALS_DIR						=	./08_signals/
CORE_DIR						=	./09_core/
DEBUG_DIR						=	./10_debug/
MODULES_DIR_ALL					=	$(LEXER_DIR)\
									$(PARSER_DIR)\
									$(BUILTINS_DIR)\
									$(CORE_DIR)\
									$(EXPANDER_DIR)\
									$(DEBUG_DIR)\
									$(EXECUTOR_DIR)\
									$(REDIRECTOR_DIR)\
									#$(SIGNALS_DIR)#
SUBMODULE						=	submodule_initialised

#	Libraries
GET_NEXT_LINE					=	$(LIBALLME_DIR)libgnl.a
LIBME							=	$(LIBME_DIR)libme.a
LEXER							=	$(LEXER_DIR)lexer.a
PARSER							=	$(PARSER_DIR)parser.a
EXPANDER						=	$(EXPANDER_DIR)expander.a
REDIRECTOR						=	$(REDIRECTOR_DIR)redirector.a
EXECUTOR						=	$(EXECUTOR_DIR)executor.a
BUILTINS						=	$(BUILTINS_DIR)builtins.a
SIGNALS							=	$(SIGNALS_DIR)signals.a
CORE							=	$(CORE_DIR)core.a
DEBUG							=	$(DEBUG_DIR)debug.a
MODULES_ALL						=	$(GET_NEXT_LINE)\
									$(LIBME)\
									$(LEXER)\
									$(PARSER)\
									$(BUILTINS)\
									$(CORE)\
									$(EXPANDER)\
									$(DEBUG)\
									$(EXECUTOR)\
									$(REDIRECTOR)\
									#$(SIGNALS)#

#	General Rules
CC								=	cc
CC_DEBUG						=	$(shell echo $$DEBUG_FLAG)
CFLAGS							=	-Wall -Wextra -Werror $(CC_DEBUG)
BREW_PATH_IF  					=	if test -d /Users/$(USER)/.brew; \
									then echo /Users/$(USER)/.brew/; \
									elif test -d /Users/$(USER)/goinfre/.brew; \
               						then echo /Users/$(USER)/goinfre/.brew/; \
									else echo ""; fi
BREW_PATH 						:= 	$(shell $(BREW_PATH_IF))
RL_LINK  						=	-L $(BREW_PATH)opt/readline/lib -lreadline
REMOVE							=	rm -f

#	Deletes targets on error
.DELETE_ON_ERROR:

#	General targets
.PHONY:									all clean fclean re ref update valgrind linux
all:									$(SUBMODULE) $(NAME)
$(NAME):								$(MODULES_ALL)
										$(CC) $(CFLAGS) -o $(NAME) $(RL_LINK)
valgrind:								linux
											valgrind --leak-check=full \
											--show-leak-kinds=all \
											--fullpath-after= \
											--track-origins=yes \
											--log-file=./valgrind_debug.log \
											--show-reachable=yes \
											--trace-children=yes \
											--verbose \
											./minishell							
linux:									$(MODULES_ALL)
											cp $(GET_NEXT_LINE) ./
											cp $(LIBME) ./
											cp $(LEXER) ./
											cp $(PARSER) ./
											cp $(EXPANDER) ./
											cp $(REDIRECTOR) ./
											cp $(EXECUTOR) ./
											cp $(BUILTINS) ./
											cp $(CORE) ./
											cp $(DEBUG) ./
											ar -x libgnl.a
											ar -x libme.a
											ar -x lexer.a
											ar -x parser.a
											ar -x expander.a
											ar -x redirector.a
											ar -x executor.a
											ar -x builtins.a
											ar -x core.a
											ar -x debug.a
											rm libgnl.a
											rm libme.a
											rm lexer.a
											rm parser.a
											rm expander.a
											rm redirector.a
											rm executor.a
											rm builtins.a
											rm core.a
											rm debug.a
											$(CC) $(CFLAGS) *.o -o $(NAME) -lreadline
											rm *.o
$(MODULES_ALL):
											$(MAKE) gnl -C $(LIBALLME_DIR)
											$(MAKE) libme -C $(LIBALLME_DIR)
											for dir in $(MODULES_DIR_ALL); do\
												$(MAKE) -C $$dir; \
												done
clean:
											$(MAKE) clean -C $(LIBALLME_DIR)
											for dir in $(MODULES_DIR_ALL); do\
												$(MAKE) clean -C $$dir; \
												done
fclean:									clean
											$(MAKE) fclean -C $(LIBALLME_DIR)
											for dir in $(MODULES_DIR_ALL); do\
												$(MAKE) fclean -C $$dir; \
												done
											$(REMOVE) $(NAME)
											$(REMOVE) $(SUBMODULE)
re:										fclean
											$(MAKE)
ref:
											for dir in $(MODULES_DIR_ALL); do\
												$(MAKE) fclean -C $$dir; \
												done
											$(REMOVE) $(NAME)
											$(MAKE)
$(SUBMODULE):
											git submodule update --init \
												--recursive --remote
											touch $(SUBMODULE)
											$(MAKE) update
update:
											git submodule foreach git pull \
												origin master
											git submodule foreach git checkout \
												master
