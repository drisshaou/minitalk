/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drhaouha <drhaouha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 21:06:11 by drhaouha          #+#    #+#             */
/*   Updated: 2024/07/03 04:40:30 by drhaouha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_H
# define CLIENT_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <signal.h>
# include <limits.h>

void	handle_sig(int sig, siginfo_t *info, void *context);
void	send_bit(int bit, pid_t server_pid);
void	send_byte(unsigned char byte, pid_t server_pid);
void	send_text(unsigned char *text, pid_t server_pid);
size_t	ft_strlen(unsigned char *str);
int		ft_atoi(const char *s);

#endif