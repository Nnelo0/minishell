NAME = minishell
CC = cc
CFLAGS = -Wall -Werror -Wextra -g
SRCS = src/commands_utils.c src/commands.c src/env.c src/ft_builtins.c \
src/ft_echo_utils.c src/ft_echo.c src/ft_export_utils.c src/ft_export.c \
src/ft_heredoc.c src/ft_parse_redirection.c src/ft_path_utils.c src/ft_path.c \
src/ft_pipe.c src/ft_redirections.c src/ft_shell.c src/ft_split_quote.c \
src/ft_unset.c src/get_env.c src/get_export.c src/get_utils.c \
src/main.c src/utils.c src/verif.c \
 
LIBFTDIR = libft
LIBFT = $(LIBFTDIR)/libft.a 

all: $(LIBFT) $(NAME)

$(NAME): $(SRCS) $(LIBFT)
	$(CC) -I. -I$(LIBFTDIR) $(SRCS) $(LIBFT) $(CFLAGS) -o $(NAME) -lreadline
	@echo "Compiled $(NAME) successfully!"

$(LIBFT):
	$(MAKE) -C $(LIBFTDIR)

clean:
	$(MAKE) -C $(LIBFTDIR) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFTDIR) fclean

re: fclean all

.PHONY: all clean fclean re