/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drhaouha <drhaouha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 21:06:11 by drhaouha          #+#    #+#             */
/*   Updated: 2024/07/03 02:37:21 by drhaouha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_H
# define SERVER_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <signal.h>
# include <string.h>

typedef struct s_buffer
{
	int				bit_value;
	int				bit_count;
	unsigned char	current_byte;
	int				byte_count;
	unsigned char	buffer[512];
}	t_buffer;

unsigned char	*update_str(unsigned char **str, t_buffer *data);
void			print_str(unsigned char **str);
void			process_byte(siginfo_t *info, t_buffer *data,
					unsigned char **str);
void			handle_bit(int sig, siginfo_t *info, void *context);
void			*ft_memset(void *b, int c, size_t len);
size_t			ft_strlen(unsigned char *str);
void			ft_putnbr(int n);
void			ft_putchar(char c);

#endif