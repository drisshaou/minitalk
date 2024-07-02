/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drhaouha <drhaouha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 21:06:11 by drhaouha          #+#    #+#             */
/*   Updated: 2024/07/02 03:41:05 by drhaouha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_H
# define SERVER_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <signal.h>
# include <string.h>

typedef struct s_utf8
{
	int				bit_value;
	int				bit_count;
	unsigned char	current_byte;
	int				byte_count;
	unsigned char	utf8_char[4];
	unsigned char	stock[256];
}	t_utf8;

unsigned char	*fill_str(unsigned char **str, unsigned char utf8_char[], int byte_count);

// unsigned char	*fill_str(unsigned char **str, unsigned char stock[], int byte_count);
// unsigned char	*fill_stock(unsigned char **str, t_utf8 *ctx);

void	print_str(unsigned char **str);
void	fill_utf8_char(siginfo_t *siginfo, t_utf8 *ctx, unsigned char **str);
void	handle_bit(int sig, siginfo_t *siginfo, void *context);
void	*ft_memset(void *b, int c, size_t len);
size_t	ft_strlen(unsigned char *str);
void	ft_putnbr(int n);
void	ft_putchar(char c);

#endif