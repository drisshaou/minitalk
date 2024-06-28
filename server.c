#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
// #include <sys/siginfo.h>


// pid_t clent_pid;

// Déclaration du gestionnaire de signal avec des informations supplémentaires
// void handle_sigint(int sig, siginfo_t *siginfo, void *context) {
//     printf("Caught signal %d (SIGINT)\n", sig);
//     printf("Signal number: %d\n", siginfo->si_signo);
//     printf("Sending PID: %d\n", siginfo->si_pid);
//     printf("User time consumed: %ld\n", siginfo->si_utime);
//     printf("System time consumed: %ld\n", siginfo->si_stime);
//     exit(0);
// }

void handle_bit(int sig, siginfo_t *siginfo, void *context) {
	(void)context;
    static int bitValue = -1;
    static int bitCount = 0;
    static char currentChar = 0;

	printf("received: %d\n\n", sig);

    if (sig == SIGUSR1) {
        bitValue = 0;
    } else if (sig == SIGUSR2) {
        bitValue = 1;
    }

    currentChar = (currentChar << 1) | bitValue;
    bitCount++;

    if (bitCount == 8) {
        printf("%c", currentChar);
        // fflush(stdout);
        bitCount = 0;
        currentChar = 0;
        // kill(siginfo->si_pid, SIGUSR1); // Envoyer confirmation de réception
    }
    kill(siginfo->si_pid, SIGUSR2); // Envoyer confirmation de réception
	pause();
}

int main() {
	// pid_t clent_pid;
    struct sigaction sa1, sa2;

    // sa1.sa_handler = handle_bit;
    sa1.sa_sigaction = handle_bit;
    sa1.sa_flags = 0; //SA_RESTART;
    sigemptyset(&sa1.sa_mask);
    if (sigaction(SIGUSR1, &sa1, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    // sa1.sa_handler = handle_bit;
    sa1.sa_sigaction = handle_bit;
    sa2.sa_flags = 0; //SA_RESTART;
    sigemptyset(&sa2.sa_mask);
    if (sigaction(SIGUSR2, &sa2, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    printf("Receiver PID: %d\n", getpid());
    // printf("Please set the sender PID: ");
    // scanf("%d", &clent_pid);

    while (1) {
        pause();  // Attendre un signal
    }

    return 0;
}
