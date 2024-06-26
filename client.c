#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

#define MESSAGE_SIZE 256

volatile sig_atomic_t ready_to_send = 0;
char message[MESSAGE_SIZE];

void handle_signal(int sig) {
    ready_to_send = 1;
}

int main() {
    pid_t reader_pid;
    printf("Enter the reader PID: ");
    scanf("%d", &reader_pid);

    // Définir le gestionnaire de signal pour SIGUSR2
    signal(SIGUSR2, handle_signal);

    // Lire le message à envoyer
    printf("Enter the message to send: ");
    scanf("%s", message);

    // Envoyer le signal au processus lecteur
    kill(reader_pid, SIGUSR1);

    // Attendre que le lecteur soit prêt à recevoir
    while (!ready_to_send) {
        pause();
    }

    printf("Message sent: %s\n", message);
    return 0;
}
