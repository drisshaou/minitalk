/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drhaouha <drhaouha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 02:44:29 by drhaouha          #+#    #+#             */
/*   Updated: 2024/07/01 21:59:16 by drhaouha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

char	*fill_str(char **str, unsigned char utf8_char[], int byte_count)
{
	char	*tmp;
	int		len;
	int		i;

	tmp = *str;
	len = ft_strlen(*str);
	*str = (char *)malloc(sizeof(char) * (len + byte_count + 1));
	if (!(*str))
		return (NULL);
	i = -1;
	while (tmp[++i])
		(*str)[i] = tmp[i];
	i = -1;
	while (++i < byte_count)
		(*str)[len + i] = utf8_char[i];
	(*str)[len + i] = '\0';
	free(tmp);
	return (*str);
}

void	print_str(char **str)
{
	write(STDOUT_FILENO, *str, ft_strlen(*str));
	free(*str);
	*str = NULL;
}

void	fill_utf8_char(siginfo_t *siginfo, t_utf8 *ctx, char **str)
{
	ctx->utf8_char[ctx->byte_count] = ctx->current_byte;
	ctx->byte_count++;
	ctx->bit_count = 0;
	ctx->current_byte = 0;
	if (((ctx->utf8_char[0] & 0x80) == 0)
		|| ((ctx->utf8_char[0] & 0xE0) == 0xC0 && ctx->byte_count == 2)
		|| ((ctx->utf8_char[0] & 0xF0) == 0xE0 && ctx->byte_count == 3)
		|| ((ctx->utf8_char[0] & 0xF8) == 0xF0 && ctx->byte_count == 4))
	{
		if (ctx->utf8_char[0] == '\0')
			print_str(str);
		else
		{
			*str = fill_str(str, ctx->utf8_char, ctx->byte_count);
			if (!(*str))
			{
				kill(siginfo->si_pid, SIGUSR1);
				exit(EXIT_FAILURE);
			}
		}
		ctx->byte_count = 0;
		ft_memset(ctx->utf8_char, 0, 4);
	}
}

void	handle_bit(int sig, siginfo_t *siginfo, void *context)
{
	static t_utf8	ctx = {0, 0, 0, 0, {'\0', '\0', '\0', '\0'}};
	static char		*str;

	(void)context;
	if (str == NULL)
	{
		str = (char *)malloc(sizeof(char) * 1);
		if (!str)
		{
			kill(siginfo->si_pid, SIGUSR1);
			exit(EXIT_FAILURE);
		}
		str[0] = '\0';
	}
	ctx.bit_value = (sig == SIGUSR2);
	ctx.current_byte = (ctx.current_byte << 1) | ctx.bit_value;
	ctx.bit_count++;
	if (ctx.bit_count == 8)
		fill_utf8_char(siginfo, &ctx, &str);
	usleep(100);
	kill(siginfo->si_pid, SIGUSR2);
}

int	main(void)

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

