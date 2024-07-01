# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: drhaouha <drhaouha@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/16 15:15:07 by drhaouha          #+#    #+#              #
#    Updated: 2024/07/01 23:02:03 by drhaouha         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

GREEN			=	\033[0;32m
ORANGE			=	\033[0;33m
RED				=	\033[0;31m
NC				=	\033[0m

CLIENT			=	client
SERVER			=	server

SRCS_CLIENT		=	client.c \
					utils.c
SRCS_SERVER		=	server.c \
					utils.c

CC				=	gcc
CFLAGS			=	-Wall -Wextra -Werror
H_CLIENT		=	client.h
H_SERVER		=	server.h

${CLIENT}:	${SRCS_CLIENT} ${H_CLIENT}
			@${CC} ${CFLAGS} $^ -I./ -o $@
			@echo "${GREEN}$@ is created${NC}"

${SERVER}:	${SRCS_SERVER} ${H_SERVER}
			@${CC} ${CFLAGS} $^ -I./ -o $@
			@echo "${GREEN}$@ is created${NC}"

all:		${CLIENT} ${SERVER}

fclean:
			@rm -f ${CLIENT} ${SERVER}
			@echo "${RED}${CLIENT}, ${SERVER}  is removed.${NC}"

re:			fclean ${CLIENT} ${SERVER}

.PHONY:		fclean clean re all bonus