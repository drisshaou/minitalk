/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drhaouha <drhaouha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 21:34:23 by drhaouha          #+#    #+#             */
/*   Updated: 2024/07/01 22:48:57 by drhaouha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <limits.h>

void	*ft_memset(void *b, int c, size_t len)
{
	unsigned char	*cur;

	cur = (unsigned char *)b;
	while (len-- > 0)
		*(cur++) = (unsigned char)c;
	return (b);
}

size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

void	ft_putchar(char c)
{
	write(1, &c, sizeof(char));
}

void	ft_putnbr(int n)
{
	unsigned int	nbr;

	nbr = 0;
	if (n < 0)
	{
		ft_putchar('-');
		nbr = -n;
	}
	else
		nbr = n;
	if (nbr >= 10)
	{
		ft_putnbr(nbr / 10);
		ft_putnbr(nbr % 10);
	}
	else
		ft_putchar(nbr + '0');
}

int	ft_atoi(const char *s)
{
	int						sign;
	unsigned long long int	r;

	r = 0;
	sign = 1;
	while (*s == 32 || (*s >= 9 && *s <= 13))
		s++;
	if (*s == '-')
	{
		sign *= -1;
		s++;
	}
	else if (*s == '+')
		s++;
	while (*s >= '0' && *s <= '9')
	{
		r = (r * 10) + (*s - '0');
		s++;
	}
	if (r > LONG_MAX && sign < 0)
		return (0);
	else if (r > LONG_MAX && sign > 0)
		return (-1);
	return ((int)(r * sign));
}
