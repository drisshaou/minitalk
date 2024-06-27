#include <stdio.h>
#include <stdlib.h>
// #include <sys/types.h>
#include <signal.h>
#include <unistd.h>

#define MESSAGE_SIZE 256

extern char message[MESSAGE_SIZE];
volatile sig_atomic_t message_received = 0;

void handle_signal(int sig) {
	int		kill_ret;

	if (sig == SIGUSR1) {
		// Afficher le message
		printf("Received message: %s\n", message);
		message_received = 1;

		// Répondre au client
		kill_ret = kill(getppid(), SIGUSR2);
	}
}

int main() {
	// Afficher le PID pour que le client puisse l'utiliser
	printf("Server PID: %d\n", getpid());

	// Définir le gestionnaire de signal pour SIGUSR1 => (bit = 0)
	signal(SIGUSR1, handle_signal);
	// Définir le gestionnaire de signal pour SIGUSR2 => (bit = 1)
	signal(SIGUSR2, handle_signal);

	// Attendre que le signal arrive
	while (!message_received) {
		pause(); // Attendre un signal
	}

	return (0);
}
