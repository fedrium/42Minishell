SRCS		= main.c

OBJS		= $(SRCS:.c=.o)

NAME		= minishell

CC			= gcc

CFLAGS		= -Wall -Wextra -Werror

RM			= rm -rf

%.c			= %.o 

all:		${NAME}

${NAME}:	${OBJS}
				${CC} ${CFLAGS} ${OBJS} -o ${NAME} -lreadline

clean:		
				${RM} ${OBJS}

fclean:		clean
				${RM} ${NAME} ${BNAME}

re:			clean all

.PHONY:		all clean fclean re