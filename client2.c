/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drhaouha <drhaouha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 02:45:00 by drhaouha          #+#    #+#             */
/*   Updated: 2024/07/01 04:06:12 by drhaouha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdbool.h>


void handle_sig(int sig, siginfo_t *siginfo, void *context) {
    (void)siginfo;
    (void)context;
    static int bit_count_client = 0;
    static int bit_value_client = 0;
    static int bit_count_server = 0;
    static int bit_value_server = 0;
    static bool confirmed = false;

    write(1, "\nRecu\n", 6);

    if (siginfo->si_pid == getpid()) {
        bit_value_client = (sig == SIGUSR2);
        bit_value_server = !(sig == SIGUSR2);
        bit_count_client++;
    }
    else {
        bit_value_server = (sig == SIGUSR2);
        bit_count_server++;
    }
    printf("bit_count_client: %d\n", bit_count_client);
    confirmed = bit_value_client == bit_value_server && bit_count_client == bit_count_server;
    if (!confirmed && 0)
        pause();
}

void send_bit(int bit, pid_t server_pid) {
    if (bit == 0) {
        kill(server_pid, SIGUSR1);
    } else {
        kill(server_pid, SIGUSR2);
    }
    // usleep(10000); // Pause pour permettre au serveur de traiter le signal
    write(1, "\nEnvoyer\n", 9);

    // // Attendre l'accusé de réception
    // ctx->confirmed = 0;
    // while (!ctx->confirmed) {
    //     pause(); // Attendre le signal de confirmation
    // }
}

void send_byte(unsigned char byte, pid_t server_pid) {
    for (int bit = 7; bit >= 0; --bit) {
        int bit_value = (byte >> bit) & 1;
        // ctx->sended_bit = bit_value;
        // usleep(500); 
        send_bit(bit_value, server_pid);
        pause();
    }
}

void send_text_bit_by_bit(const char *text, pid_t server_pid) {
    struct sigaction sa;

    sa.sa_sigaction = handle_sig;
    sa.sa_flags = SA_SIGINFO;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGUSR1, &sa, NULL) == -1
        || sigaction(SIGUSR2, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    size_t len = strlen(text);
    for (size_t i = 0; i < len; ++i) {
        send_byte((unsigned char)text[i], server_pid);
    }

    // Envoyer le caractère nul pour indiquer la fin de la transmission
    send_byte('\0', server_pid);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <server_pid> <text>\n", argv[0]);
        return 1;
    }

    send_text_bit_by_bit(argv[2], (pid_t)atoi(argv[1]));

    return (0);
}



























/* 
typedef struct {
    pid_t server_pid;
    int sended_bit;
    volatile sig_atomic_t confirmed;
} Context;

void handle_sig(int sig, siginfo_t *siginfo, void *context) {
    (void)siginfo;
    Context *ctx = (Context *)context;
    int returned_bit;

    write(1, "\nRecu\n", 6);

    returned_bit = (sig == SIGUSR2);
    if (ctx->sended_bit == returned_bit)
        ctx->confirmed = 1;
}

void send_bit(int bit, Context *ctx) {
    if (bit == 0) {
        kill(ctx->server_pid, SIGUSR1);
    } else {
        kill(ctx->server_pid, SIGUSR2);
    }
    // usleep(1000); // Pause pour permettre au serveur de traiter le signal
    write(1, "\nEnvoyer\n", 9);

    // Attendre l'accusé de réception
    ctx->confirmed = 0;
    while (!ctx->confirmed) {
        pause(); // Attendre le signal de confirmation
    }
}

void send_byte(unsigned char byte, Context *ctx) {
    for (int bit = 7; bit >= 0; --bit) {
        int bit_value = (byte >> bit) & 1;
        ctx->sended_bit = bit_value;
        send_bit(bit_value, ctx);
    }
}

void send_text_bit_by_bit(const char *text, Context *ctx) {
    struct sigaction sa1, sa2;

    sa1.sa_sigaction = handle_sig;
    sa1.sa_flags = SA_SIGINFO;
    sigemptyset(&sa1.sa_mask);
    if (sigaction(SIGUSR1, &sa1, (void *)ctx) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    sa2.sa_sigaction = handle_sig;
    sa2.sa_flags = SA_SIGINFO;
    sigemptyset(&sa2.sa_mask);
    if (sigaction(SIGUSR2, &sa2, (void *)ctx) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    size_t len = strlen(text);
    for (size_t i = 0; i < len; ++i) {
        send_byte((unsigned char)text[i], ctx);
    }

    // Envoyer le caractère nul pour indiquer la fin de la transmission
    send_byte('\0', ctx);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <server_pid> <text>\n", argv[0]);
        return 1;
    }

    Context *ctx = (Context *)malloc(sizeof(Context));
    if (ctx == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    ctx->sended_bit = 0;
    ctx->server_pid = (pid_t)atoi(argv[1]);
    ctx->confirmed = 0;

    const char *text = argv[2];

    send_text_bit_by_bit(text, ctx);

    free(ctx);
    return 0;
}
 */