#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
// #include <sys/siginfo.h>

#include <limits.h>
#include <stdint.h>
#include <stddef.h>


// Déclaration du gestionnaire de signal avec des informations supplémentaires
// void handle_sigint(int sig, siginfo_t *siginfo, void *context) {
//     printf("Caught signal %d (SIGINT)\n", sig);
//     printf("Signal number: %d\n", siginfo->si_signo);
//     printf("Sending PID: %d\n", siginfo->si_pid);
//     printf("User time consumed: %ld\n", siginfo->si_utime);
//     printf("System time consumed: %ld\n", siginfo->si_stime);
//     exit(0);
// }

// void handle_error(int sig, siginfo_t *siginfo, void *context) {
// 	(void)sig;
// 	(void)siginfo;
// 	(void)context;
//     perror("sigaction");
//     exit(EXIT_FAILURE);
// }

volatile sig_atomic_t confirmed;

void handle_ack(int sig, siginfo_t *siginfo, void *context) {
	(void)sig;
	(void)siginfo;
	(void)context;
    static int ack_received = 0;
    ack_received = 1;
	printf("ack_received: %d\n", ack_received);
    printf("received from server: %d\n\n", sig);
    confirmed = 1;
}

void load_sigaction() {
    struct sigaction sa1, sa2;
	sa1.sa_sigaction = handle_ack;
    sa1.sa_flags = SA_SIGINFO | SA_RESTART;
    sigemptyset(&sa1.sa_mask);
    if (sigaction(SIGUSR1, &sa1, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

	sa2.sa_sigaction = handle_ack;
    sa2.sa_flags = SA_SIGINFO | SA_RESTART;
    sigemptyset(&sa2.sa_mask);
    if (sigaction(SIGUSR2, &sa2, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }
}

void sendBit(int bit, pid_t server_pid) {
    if (bit == 0) {
        printf("send to server: %d\n\n", bit);
        kill(server_pid, SIGUSR1);
    } else {
        printf("send to server: %d\n\n", bit);
        kill(server_pid, SIGUSR2);
    }
    // usleep(500); // Pause pour permettre au serveur de traiter le signal
    // pause();
    confirmed = 0;
    while (!confirmed) {
        printf("paused...\n");
        pause();
    }
}

void sendTextBitByBit(const char *text, pid_t server_pid) {
    load_sigaction();

    for (size_t i = 0; i < strlen(text); ++i) {
        char ch = text[i];
        for (int bit = 7; bit >= 0; --bit) {
            int bitValue = (ch >> bit) & 1;
            // load_sigaction();
            sendBit(bitValue, server_pid);
			// pause();
        }
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

    confirmed = 0;

    server_pid = (pid_t)ft_atoi(argv[1]);
    const char *text = argv[2];

    sendTextBitByBit(text, server_pid);
    return 0;
}
