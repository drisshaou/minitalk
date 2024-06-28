#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

#include <limits.h>
#include <stdint.h>
#include <stddef.h>

// #define MESSAGE_SIZE 256

volatile sig_atomic_t ready_to_send = 0;
// char message[MESSAGE_SIZE];

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

unsigned char reverse_bits(unsigned char octet) {
	unsigned char octet1;
	int	i;

	i = -1;
	octet1 = 0;
	while (++i <= 7)
		octet1 |= ((octet >> i) & 1) << (7 - i);
	return (octet1);
}

void print_bits(unsigned char byte) {
	int	i = 8;
	unsigned char octet[i];

	while(--i >= 0){
		octet[i] = (byte % 2) + '0';
		byte /= 2;
	}
	write(1, octet, 8);
}

void handle_signal(int sig) {
	ready_to_send = 1;
}

int main(int ac, char **av) {
	pid_t	server_pid;
	char	*message;
	int		kill_ret;

	if (ac == 3) {
		// printf("Enter the reader PID: ");
		// scanf("%d", &server_pid);
		server_pid = (pid_t)ft_atoi(av[1]);
		message = av[2];

		// Définir le gestionnaire de signal pour SIGUSR1 => (bit = 0)
		signal(SIGUSR1, handle_signal);
		// Définir le gestionnaire de signal pour SIGUSR2 => (bit = 1)
		signal(SIGUSR2, handle_signal);

		// Lire le message à envoyer
		// printf("Enter the message to send: ");
		// scanf("%s", message);

		// Envoyer le signal au processus serveur
		kill_ret = kill(server_pid, SIGUSR1);

		// Attendre que le serveur soit prêt à recevoir
		while (!ready_to_send) {
			pause();
		}

		// printf("Message sent: %s\n", message);
	}
	return (0);
}
