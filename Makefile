SRCS		= main.c

OBJS		= $(SRCS:.c=.o)

NAME		= minishell

CC			= gcc

CFLAGS		= -Wall -Wextra -Werror

RM			= rm -rf

%.c			= %.o 

all:		${NAME}

${NAME}:		${OBJS}
				make libft
				${CC} libft.a ${CFLAGS} ${OBJS} -o ${NAME} -lreadline

libft:		
				make -C libft
				cp libft/libft.a .

clean:		
				${RM} ${OBJS}
				make clean -C libft

fclean:		clean
				${RM} ${NAME} ${BNAME}
				make fclean -C libft

re:			clean all
			make re -C libft

.PHONY:		all clean fclean re