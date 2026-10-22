# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: eduzun <eduzun@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/09/02 09:06:47 by eduzun            #+#    #+#              #
#    Updated: 2025/09/02 09:06:53 by eduzun           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#===========================#
#           COLORS          #
#===========================#
PINK       = \033[38;5;218m
BOLD       = \033[1m
RESET      = \033[0m
SUCCESS    = 🍓 $(PINK)Compilation finished 🍓$(RESET)
COMPILE    = $(PINK)🍓 Compiling... 🍓$(RESET)
CLEAN_MSG  = $(PINK)🍓 Cleaning up compiled files 🍓$(RESET)
FCLEAN_MSG = $(PINK)🍓 A clean shell is a happy shell 🍓$(RESET)

#===========================#
#        COMPILATION        #
#===========================#
NAME    = minishell
CC      = cc
CFLAGS  = -Wall -Wextra -Werror -g

SRC_DIR = src
INC_DIR = includes
OBJ_DIR = obj

SRC = src/prompt/prompt.c\
	  src/prompt/builtin_handler.c\
	  src/prompt/heredoc_handler.c\
	  src/prompt/heredoc_utils.c\
	  src/signals/helpful_signal.c\
	  src/signals/signal_ctrl_c.c\
	  src/builtins/builtin_echo.c\
	  src/builtins/builtin_env.c\
	  src/builtins/builtin_cd.c\
	  src/builtins/cd_utils.c\
	  src/builtins/builtin_unset.c\
	  src/builtins/builtin_export.c\
	  src/builtins/export_trie.c\
	  src/builtins/export_utils.c\
	  src/builtins/builtin_pwd.c\
	  src/builtins/builtin_exit.c\
	  src/builtins/utils_builtins.c\
	  src/builtins/ft_echo_expansions.c\
	  src/parsing/parsing_quotes.c\
	  src/parsing/token_extractors_word.c\
	  src/parsing/token_extractors_operator.c\
	  src/parsing/token_helpers.c\
	  src/parsing/token_utils.c\
	  src/parsing/token_manager.c\
	  src/libft/string_utils_one.c\
	  src/libft/string_utils_two.c\
	  src/libft/string_utils_three.c\
	  src/libft/string_utils_four.c\
	  src/execution/path/paths_split.c\
	  src/execution/path/get_command_path.c\
	  src/execution/path/absolute_path.c\
	  src/execution/path/expansion.c\
	  src/execution/path/executer_commande.c\
	  src/execution/path/path_utils.c\
	  src/execution/path/ft_split_utils.c\
	  src/execution/path/argv_builder.c\
	  src/execution/path/error_utils.c\
	  src/execution/path/exec_utils.c\
	  src/execution/path/path_commande.c\
	  src/execution/redirection/redir_append.c\
	  src/execution/redirection/heredoc_loop.c\
	  src/execution/redirection/heredoc_utils.c\
	  src/execution/redirection/handle_heredoc.c\
	  src/execution/redirection/redir_utils.c\
	  src/execution/redirection/heredoc_signals.c\
	  src/execution/redirection/redirection.c\
	  src/execution/pipe/pipex_launch.c\
	  src/execution/pipe/pipex_pipe.c\
	  src/execution/pipe/pipex_child_setup.c\
	  src/execution/pipe/pipex_child_exec.c\
	  src/execution/pipe/pipe_utils.c\
	  src/execution/pipe/parent_process.c\
	  src/init.c\


OBJS = $(SRC:src/%.c=$(OBJ_DIR)/%.o)

LIBS = -lreadline

#===========================#
#           RULES           #
#===========================#
all: $(NAME)

$(NAME): $(OBJS)
	@echo  "$(COMPILE)"
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LIBS)
	@echo "$(SUCCESS)"

$(OBJ_DIR)/%.o: src/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

clean:
	@echo  "$(CLEAN_MSG)"
	@rm -rf $(OBJ_DIR)

fclean: clean
	@echo  "$(FCLEAN_MSG)"
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
