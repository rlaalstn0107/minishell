NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -g
RM = rm
RMFLAGS = -f

INCLUDE = ./include

LIBFT = ./libft

LIBFT_FLAG = -Llibft -lft

SRCDIR = ./src

SRCS	= ft_tokenizer.c
SRCS	:= $(addprefix $(SRCDIR)/, $(SRCS))

OBJS = $(SRCS:.c=.o)


all : $(NAME)

$(NAME): $(OBJS)
	make -C $(LIBFT) bonus
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT_FLAG) -o $(NAME)

%.o	: %.c
	$(CC) $(CFLAGS) $(HEADERS) -Ilibft -c $< -o $@

bonus: 
	make DO_BONUS=1 all
	
clean:
	make -C $(LIBFT) clean
	$(RM) $(RMFLAGS) $(OBJS) $(BONUS_OBJS)

fclean: clean
	$(RM) $(RMFLAGS) $(LIBFT)/libft.a
	$(RM) $(RMFLAGS) $(NAME)

re: 
	make fclean 
	make all

.PHONY: all bonus clean fclean re

