/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drhaouha <drhaouha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 02:44:29 by drhaouha          #+#    #+#             */
/*   Updated: 2024/07/02 21:36:53 by drhaouha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

unsigned char	*update_str(unsigned char **str, t_stock *stock)
{
	unsigned char	*tmp;
	int				len;
	int				i;

	tmp = *str;
	len = ft_strlen(*str);
	*str = (unsigned char *)malloc(sizeof(char) * (len + stock->byte_count + 1));
	if (!(*str))
		return (NULL);
	i = -1;
	while (tmp[++i])
		(*str)[i] = tmp[i];
	i = -1;
	while (++i < stock->byte_count)
		(*str)[len + i] = stock->buffer[i];
	(*str)[len + i] = '\0';
	free(tmp);
	return (*str);
}

void	print_str(unsigned char **str)
{
	write(STDOUT_FILENO, *str, ft_strlen(*str));
	free(*str);
	*str = NULL;
}

void	process_byte(siginfo_t *siginfo, t_stock *stock, unsigned char **str)
{
	stock->buffer[stock->byte_count] = stock->current_byte;
	stock->byte_count++;
	stock->bit_count = 0;
	stock->current_byte = 0;
	if (stock->buffer[stock->byte_count - 1] == '\0' || stock->byte_count == 512)
	{
		*str = update_str(str, stock);
		if (!(*str))
		{
			kill(siginfo->si_pid, SIGUSR1);
			exit(EXIT_FAILURE);
		}
		if (stock->buffer[stock->byte_count - 1] == '\0')
			print_str(str);
		stock->byte_count = 0;
		ft_memset(stock->buffer, 0, 512);
	}
}

void	handle_bit(int sig, siginfo_t *siginfo, void *context)
{
	static t_stock			stock = {0, 0, 0, 0, {'\0'}};
	static unsigned char	*str;

	(void)context;
	if (str == NULL)
	{
		str = (unsigned char *)malloc(sizeof(char) * 1);
		if (!str)
		{
			kill(siginfo->si_pid, SIGUSR1);
			exit(EXIT_FAILURE);
		}
		str[0] = '\0';
	}
	stock.bit_value = (sig == SIGUSR2);
	stock.current_byte = (stock.current_byte << 1) | stock.bit_value;
	stock.bit_count++;
	if (stock.bit_count == 8)
		process_byte(siginfo, &stock, &str);
	usleep(200);
	kill(siginfo->si_pid, SIGUSR2);
}

int	main(void)
{
	struct sigaction	sa;

	write(STDOUT_FILENO, "Server PID: ", 12);
	ft_putnbr(getpid());
	write(STDOUT_FILENO, "\n", 1);
	sa.sa_sigaction = handle_bit;
	sa.sa_flags = SA_SIGINFO;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGUSR1, &sa, NULL) == -1
		|| sigaction(SIGUSR2, &sa, NULL) == -1)
	{
		exit(EXIT_FAILURE);
	}
	while (1)
		pause();
	return (0);
}
