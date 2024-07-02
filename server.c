/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drhaouha <drhaouha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 02:44:29 by drhaouha          #+#    #+#             */
/*   Updated: 2024/07/02 20:30:37 by drhaouha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

unsigned char	*update_str(unsigned char **str, t_utf8 *ctx)
{
	unsigned char	*tmp;
	int				len;
	int				i;

	tmp = *str;
	len = ft_strlen(*str);
	*str = (unsigned char *)malloc(sizeof(char) * (len + ctx->byte_count + 1));
	if (!(*str))
		return (NULL);
	i = -1;
	while (tmp[++i])
		(*str)[i] = tmp[i];
	i = -1;
	while (++i < ctx->byte_count)
		(*str)[len + i] = ctx->stock[i];
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

void	process_byte(siginfo_t *siginfo, t_utf8 *ctx, unsigned char **str)
{
	ctx->stock[ctx->byte_count] = ctx->current_byte;
	ctx->byte_count++;
	ctx->bit_count = 0;
	ctx->current_byte = 0;
	if (ctx->stock[ctx->byte_count - 1] == '\0' || ctx->byte_count == 1024)
	{
		*str = update_str(str, ctx);
		if (!(*str))
		{
			kill(siginfo->si_pid, SIGUSR1);
			exit(EXIT_FAILURE);
		}
		if (ctx->stock[ctx->byte_count - 1] == '\0')
			print_str(str);
		ctx->byte_count = 0;
		ft_memset(ctx->stock, 0, 1024);
	}
}

void	handle_bit(int sig, siginfo_t *siginfo, void *context)
{
	static t_utf8			ctx = {0, 0, 0, 0, {'\0'}};
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
	ctx.bit_value = (sig == SIGUSR2);
	ctx.current_byte = (ctx.current_byte << 1) | ctx.bit_value;
	ctx.bit_count++;
	if (ctx.bit_count == 8)
		process_byte(siginfo, &ctx, &str);
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
