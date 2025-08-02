CC		=	cc
CFLAGS	=	-Wall -Wextra -Werror
SOURCE	=	monitor.c philo.c run_simulation.c utils_1.c utils_2.c utils_3.c
NAME	=	philo
INCLUDE	=	philo.h
OBJS	=	$(SOURCE:.c=.o)

all: $(NAME)

%.o:%.c	$(INCLUDE)
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY:	all clean fclean re
