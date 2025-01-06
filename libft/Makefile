NAME = libft.a

CC = cc

CFLAGS = -Wall -Wextra -Werror

SRCS = \
	libft/*.c \
	printf/*.c \
	gnl/get_next_line.c \

AR = ar rcs

all: $(NAME)

$(NAME):
	$(CC) $(CFLAGS) -c $(SRCS)
	$(AR) $(NAME) *.o
	@rm -f *.o
	@echo "Library $(NAME) successfully created!"

clean:
	@echo "No .o files to clean in this version."

fclean: clean
	$(RM) $(NAME)

re: fclean all
