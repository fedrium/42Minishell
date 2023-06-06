SRCS		= main.c util.c split.c

OBJS		= $(SRCS:.c=.o)

NAME		= minishell

CC			= gcc

CFLAGS		= -Wall -Wextra -Werror

RM			= rm -rf

%.c			= %.o 

all:		${NAME}

${NAME}:		${OBJS}
				${CC} ${CFLAGS} ${OBJS} -o ${NAME} -lreadline

clean:		
				${RM} ${OBJS}
				make clean -C libft

fclean:		clean
				${RM} ${NAME} ${BNAME}
				make fclean -C libft

re:			clean all

.PHONY:		all clean fclean re