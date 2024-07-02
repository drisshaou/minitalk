/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drhaouha <drhaouha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 02:45:00 by drhaouha          #+#    #+#             */
/*   Updated: 2024/07/02 18:27:22 by drhaouha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

void	handle_sig(int sig, siginfo_t *siginfo, void *context)
{
	(void)siginfo;
	(void)context;
	if (sig == SIGUSR1)
		exit(EXIT_FAILURE);
}

void	send_bit(int bit, pid_t server_pid)
{
	if (bit == 0)
		kill(server_pid, SIGUSR1);
	else
		kill(server_pid, SIGUSR2);
	// usleep(10000); // Pause pour permettre au serveur de traiter le signal
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
		// usleep(400);
		pause();
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
		return (1);
	}
	send_text((unsigned char *)av[2], (pid_t)ft_atoi(av[1]));
	return (0);
}
