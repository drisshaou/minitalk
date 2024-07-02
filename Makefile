# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: drhaouha <drhaouha@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/16 15:15:07 by drhaouha          #+#    #+#              #
#    Updated: 2024/07/02 19:02:02 by drhaouha         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

GREEN			=	\033[0;32m
ORANGE			=	\033[0;33m
RED				=	\033[0;31m
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

${LIB}:		${OBJS}
			@ar rcs $@ ${OBJS}
			@echo "${GREEN}${LIB} is created.${NC}"

${CLIENT}:	${LIB}
			@${CC} ${CFLAGS} ${SRCS_CLIENT} -I./ -L./ -lutils -o $@
			@echo "${GREEN}$@ is created${NC}"

${SERVER}:	${LIB}
			@${CC} ${CFLAGS} ${SRCS_SERVER} -I./ -L./ -lutils -o $@
			@echo "${GREEN}$@ is created${NC}"

all:		${CLIENT} ${SERVER}

clean:
			@rm -f ${OBJS} ${LIB}
			@echo "${ORANGE}${OBJS}, ${LIB} are removed.${NC}"

fclean:		clean
			@rm -f ${CLIENT} ${SERVER}
			@echo "${RED}${CLIENT}, ${SERVER}  are removed.${NC}"

re:			fclean ${CLIENT} ${SERVER}

.PHONY:		fclean clean re all bonus