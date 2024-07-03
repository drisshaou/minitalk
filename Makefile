# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: drhaouha <drhaouha@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/16 15:15:07 by drhaouha          #+#    #+#              #
#    Updated: 2024/07/03 02:58:36 by drhaouha         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

GREEN			=	\033[0;32m
ORANGE			=	\033[0;33m
RED				=	\033[0;31m
BLUE			=	\033[0;34m
NC				=	\033[0m

CC				=	gcc
CFLAGS			=	-Wall -Wextra -Werror

CLIENT			=	client
SERVER			=	server
LIB				=	libutils.a

SRCS_CLIENT		=	client.c
SRCS_SERVER		=	server.c

UTILS			=	utils.c
OBJS			=	${UTILS:.c=.o}

%.o: %.c
			@${CC} ${CFLAGS} -c $< -o $@
			@echo "${NC}$@ is created${NC}"

all:		${CLIENT} ${SERVER}

${LIB}:		${OBJS}
			@ar rcs $@ ${OBJS}
			@echo "${BLUE}${LIB} is created.${NC}"

${CLIENT}:	${LIB}
			@${CC} ${CFLAGS} ${SRCS_CLIENT} -I./ -L./ -lutils -o $@
			@echo "${GREEN}$@ is created${NC}"

${SERVER}:	${LIB}
			@${CC} ${CFLAGS} ${SRCS_SERVER} -I./ -L./ -lutils -o $@
			@echo "${GREEN}$@ is created${NC}"

clean:
			@rm -f ${OBJS} ${LIB}
			@echo "${ORANGE}${OBJS}, ${LIB} are removed.${NC}"

fclean:		clean
			@rm -f ${CLIENT} ${SERVER}
			@echo "${RED}${CLIENT}, ${SERVER}  are removed.${NC}"

re:			fclean ${CLIENT} ${SERVER}

.PHONY:		fclean clean re all bonus