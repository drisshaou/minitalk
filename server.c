/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drhaouha <drhaouha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 02:44:29 by drhaouha          #+#    #+#             */
/*   Updated: 2024/07/04 03:17:00 by drhaouha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

unsigned char	*update_str(unsigned char **str, t_buffer *data)
{
	unsigned char	*tmp;
	int				len;
	int				size;
	int				i;

	tmp = *str;
	len = ft_strlen(*str);
	size = len + data->byte_count;
	*str = (unsigned char *)malloc(sizeof(char) * (size + 1));
	if (!(*str))
		return (free(tmp), NULL);
	i = -1;
	while (tmp[++i])
		(*str)[i] = tmp[i];
	i = -1;
	while (++i < data->byte_count)
		(*str)[len + i] = data->buffer[i];
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

void	process_byte(t_buffer *data, unsigned char **str)
{
	data->buffer[data->byte_count] = data->current_byte;
	data->byte_count++;
	if (data->current_byte == '\0' || data->byte_count == 1024)
	{
		data->byte_count -= (data->current_byte == '\0');
		*str = update_str(str, data);
		if (!(*str))
			exit(EXIT_FAILURE);
		if (data->current_byte == '\0')
			print_str(str);
		ft_memset(data->buffer, 0, 1024);
		data->byte_count = 0;
	}
	data->current_byte = 0;
	data->bit_count = 0;
}

void	handle_bit(int sig, siginfo_t *info, void *context)
{
	static t_buffer			data = {0, 0, 0, 0, {'\0'}};
	static unsigned char	*str;

	(void)context;
	if (str == NULL)
	{
		str = (unsigned char *)malloc(sizeof(char) * 1);
		if (!str)
			exit(EXIT_FAILURE);
		str[0] = '\0';
	}
	data.bit_value = (sig == SIGUSR2);
	data.current_byte = (data.current_byte << 1) | data.bit_value;
	data.bit_count++;
	if (data.bit_count == 8)
		process_byte(&data, &str);
	usleep(200);
	kill(info->si_pid, sig);
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
		return (0);
	}
	while (1)
		pause();
	return (1);
}
