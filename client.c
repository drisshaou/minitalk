/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drhaouha <drhaouha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 02:45:00 by drhaouha          #+#    #+#             */
/*   Updated: 2024/07/04 03:47:09 by drhaouha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

void	handle_sig(int sig, siginfo_t *info, void *context)
{
	static t_ack	ack = {0, 0, 0, 0};

	(void)context;
	if (info->si_pid == getpid())
		ack.bit = (sig == SIGUSR2);
	else
	{
		ack.server_bit = (sig == SIGUSR2);
		if (ack.bit != ack.server_bit)
		{
			write(STDOUT_FILENO, "Bits comparison error.\n", 23);
			exit(EXIT_FAILURE);
		}
		ack.current_byte = (ack.current_byte << 1) | ack.server_bit;
		ack.bit_count++;
		if (ack.bit_count == 8 && ack.current_byte == '\0')
			write(STDOUT_FILENO, "Acknowledgment received.\n", 25);
		ack.bit_count = !(ack.bit_count == 8) * ack.bit_count;
		ack.current_byte = !(ack.bit_count == 8) * ack.current_byte;
	}
}

void	send_bit(int bit, pid_t server_pid)
{
	if (bit == 0)
		kill(getpid(), SIGUSR1);
	else
		kill(getpid(), SIGUSR2);
	if (bit == 0)
		kill(server_pid, SIGUSR1);
	else
		kill(server_pid, SIGUSR2);
}

void	send_byte(unsigned char byte, pid_t server_pid)
{
	int	bit;
	int	bit_value;

	bit = 7;
	while (bit >= 0)
	{
		bit_value = (byte >> bit) & 1;
		send_bit(bit_value, server_pid);
		if (usleep(1000000) == 0)
		{
			write(STDOUT_FILENO, "Server timeout error.\n", 22);
			exit(EXIT_FAILURE);
		}
		bit--;
	}
}

void	send_text(unsigned char *text, pid_t server_pid)
{
	struct sigaction	sa;
	size_t				len;
	size_t				i;

	sa.sa_sigaction = handle_sig;
	sa.sa_flags = SA_SIGINFO;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGUSR1, &sa, NULL) == -1
		|| sigaction(SIGUSR2, &sa, NULL) == -1)
	{
		exit(EXIT_FAILURE);
	}
	len = ft_strlen(text);
	i = 0;
	while (i < len)
		send_byte((unsigned char)text[i++], server_pid);
	send_byte('\0', server_pid);
}

int	main(int ac, char **av)
{
	if (ac != 3)
	{
		write(STDOUT_FILENO, "Usage: ", 7);
		write(STDOUT_FILENO, av[0], ft_strlen((unsigned char *)av[0]));
		write(STDOUT_FILENO, " <server_pid> <text>\n", 21);
		return (0);
	}
	send_text((unsigned char *)av[2], (pid_t)ft_atoi(av[1]));
	return (1);
}
