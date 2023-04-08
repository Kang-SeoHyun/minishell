# compile options
NAME		= miniminiminishell
CC			= cc
CFLAGS		= -Wall -Wextra -Werror #-g3 -fsanitize=address
RM			= rm -f
LIBFT		= -Llibft -lft
LDFLAGS		= -lreadline -L/${HOME}/.brew/opt/readline/lib
CPPFLAGS	= -I/${HOME}/.brew/opt/readline/include
# LDFLAGS		= -lreadline -L/opt/homebrew/opt/readline/lib
# CPPFLAGS	= -I/opt/homebrew/opt/readline/include


# source files
MS_DIR		= srcs/

UTILS		= utils/environment_variables.c \
			  utils/token.c \
			  utils/utils.c	\

LEXICAL		= lexical_analysis/lexical_analysis.c	\
			  lexical_analysis/handle_heredoc_limiter.c	\
			  lexical_analysis/handle_quotes.c \
			  lexical_analysis/handle_environment_variables.c \
			  lexical_analysis/seperate_token_by_arg.c \
			  lexical_analysis/handle_chunk.c \
			  lexical_analysis/merge_continuous_argv_token.c \
			  lexical_analysis/remove_space_token.c \


SYNTAX		= syntax_analysis/syntax_analysis.c \

EXCUTE		= excute/ms_excute.c \
			  excute/set_redirection_fd.c \

CMD_LIST	= cmd_list/create_cmd_list.c \
			  cmd_list/cmd_utils.c 		\
			  cmd_list/free_cmd_list.c \

BUILT_IN	= built_in/cd.c		\
			  built_in/echo.c	\
			  built_in/env.c	\
			  built_in/exit.c	\
			  built_in/export.c \
			  built_in/pwd.c	\
			  built_in/unset.c  \

MS_SRCS		= main.c		\
			  $(UTILS)		\
			  $(LEXICAL)	\
			  $(SYNTAX)		\
			  $(EXCUTE)		\
			  $(CMD_LIST)	\
			  $(BUILT_IN)

MS_PATH		= $(addprefix $(MS_DIR), $(MS_SRCS))
MS_OBJS		= $(MS_PATH:c=o)

%.o: %.c
	@$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

all: $(NAME)

$(NAME): $(MS_OBJS)
	@make -sC libft all
	@$(CC) $(CFLAGS) $(LDFLAGS) $(LIBFT) $^ -o $(NAME)
	@echo "╔═══════════════════════════════════════════════╗"
	@echo "║                                               ║"
	@echo "║               miniminiminishell               ║"
	@echo "║               compile finished.               ║"
	@echo "║                                               ║"
	@echo "╚═══════════════════════════════════════════════╝"

clean:
	@make -C libft clean
	@$(RM) $(MS_OBJS)
	@echo "╔═══════════════════════════════════════════════╗"
	@echo "║              make clean finished.             ║"
	@echo "╚═══════════════════════════════════════════════╝"

fclean:
	@make -C libft fclean
	@make clean
	@$(RM) $(NAME)
	@echo "╔═══════════════════════════════════════════════╗"
	@echo "║             make fclean finished.             ║"
	@echo "╚═══════════════════════════════════════════════╝"

re:
	@make fclean
	@make all

.PHONY: all clean fclean re