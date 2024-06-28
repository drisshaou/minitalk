#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
// #include <sys/siginfo.h>

#include <limits.h>
#include <stdint.h>
#include <stddef.h>

// pid_t server_pid;

// Déclaration du gestionnaire de signal avec des informations supplémentaires
// void handle_sigint(int sig, siginfo_t *siginfo, void *context) {
//     printf("Caught signal %d (SIGINT)\n", sig);
//     printf("Signal number: %d\n", siginfo->si_signo);
//     printf("Sending PID: %d\n", siginfo->si_pid);
//     printf("User time consumed: %ld\n", siginfo->si_utime);
//     printf("System time consumed: %ld\n", siginfo->si_stime);
//     exit(0);
// }

void handle_error(int sig, siginfo_t *siginfo, void *context) {
	(void)sig;
	(void)siginfo;
	(void)context;
    perror("sigaction");
    exit(EXIT_FAILURE);
}

void handle_ack(int sig) {
	(void)sig;
    static int ack_received = 0;
    ack_received = 1;
	printf("ack_received: %d\n", ack_received);
}

void sendBit(int bit, pid_t server_pid) {
    if (bit == 0) {
        kill(server_pid, SIGUSR1);
    } else {
        kill(server_pid, SIGUSR2);
    }
    usleep(1000); // Pause pour permettre au récepteur de traiter le signal
}

void sendTextBitByBit(const char *text, pid_t server_pid) {
    struct sigaction sa;
    // sa.sa_handler = handle_error;
	sa.sa_sigaction = handle_error;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    sa.sa_handler = handle_ack;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGUSR2, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < strlen(text); ++i) {
        char ch = text[i];
        for (int bit = 7; bit >= 0; --bit) {
            int bitValue = (ch >> bit) & 1;
            sendBit(bitValue, server_pid);
			pause();
        }

        // Attendre la confirmation de réception pour l'octet
        // sigset_t mask, oldmask;
        // sigemptyset(&mask);
        // sigaddset(&mask, SIGUSR1);
        // sigprocmask(SIG_BLOCK, &mask, &oldmask);

        // int ack_received = 0;
        // struct sigaction sa;
        // sa.sa_handler = handle_ack;
        // sa.sa_flags = 0;
        // sigemptyset(&sa.sa_mask);
        // sigaction(SIGUSR1, &sa, NULL);

        // while (!ack_received) {
        //     sigsuspend(&oldmask);
        // }

        // sigprocmask(SIG_UNBLOCK, &mask, NULL);
    }
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


int main(int argc, char *argv[]) {
	pid_t server_pid;
    
	if (argc != 3) {
        fprintf(stderr, "Usage: %s <server_pid> <text>\n", argv[0]);
        return 1;
    }

    server_pid = (pid_t)ft_atoi(argv[1]);
    const char *text = argv[2];

    sendTextBitByBit(text, server_pid);
    return 0;
}
