/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drhaouha <drhaouha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 02:44:29 by drhaouha          #+#    #+#             */
/*   Updated: 2024/07/01 02:55:35 by drhaouha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    int bit_value;
    int bit_count;
    unsigned char current_byte;
    int byte_count;
    unsigned char utf8_char[4];
} Context;

char *fill_str(char **str, unsigned char utf8_char[], int byte_count) {
    char    *tmp;
    int     len;
    int i;

    tmp = *str;
    len = strlen(*str);
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

void handle_bit(int sig, siginfo_t *siginfo, void *context) {
    // Context *ctx = (Context *)context;
    (void)context;
    static char *str;
    if (str == NULL) {
        str = (char*)malloc(sizeof(char) * 1);
        str[0] = '\0';
    }

    static Context ctx = {0, 0, 0, 0, {'\0', '\0', '\0', '\0'}};
    bool    complete = false;

    ctx.bit_value = (sig == SIGUSR2);
    ctx.current_byte = (ctx.current_byte << 1) | ctx.bit_value;
    ctx.bit_count++;

    printf("\nbit count : %d\n", ctx.bit_count);
    // printf("\ncurrent_byte : %c\n", ctx.current_byte);

    write(1, "\nRecu\n", 6);
    // printf("\nbit recu.\n");

    if (ctx.bit_count == 8) {
        ctx.utf8_char[ctx.byte_count] = ctx.current_byte;
        ctx.byte_count++;
        ctx.bit_count = 0;
        ctx.current_byte = 0;
        // printf("\nutf8_char : %c\n", ctx.utf8_char[0]);

        // Vérifier si le caractère est entièrement reçu (1 à 4 octets en UTF-8)
        if (((ctx.utf8_char[0] & 0x80) == 0) ||  // 1-byte character
            ((ctx.utf8_char[0] & 0xE0) == 0xC0 && ctx.byte_count == 2) ||  // 2-byte character
            ((ctx.utf8_char[0] & 0xF0) == 0xE0 && ctx.byte_count == 3) ||  // 3-byte character
            ((ctx.utf8_char[0] & 0xF8) == 0xF0 && ctx.byte_count == 4)) {  // 4-byte character

            if (ctx.utf8_char[0] == '\0') {
                printf("\nTransmission terminée, %s\n", str);
                write(STDOUT_FILENO, str, strlen(str));
                free(str);
                complete = true;
                // exit(0);
            }
            else {
                str = fill_str(&str, ctx.utf8_char, ctx.byte_count);
                // write(STDOUT_FILENO, ctx.utf8_char, ctx.byte_count);
                ctx.byte_count = 0;
                memset(ctx.utf8_char, 0, 4);
            }
        }
    }

    usleep(500); // Pause pour permettre au client de traiter le signal

    // Envoyer l'accusé de réception pour chaque bit (voir si renvoyer avant fin de transmission)
    kill(siginfo->si_pid, sig);
    
    // Transmission terminée.
    if (complete)
        exit(0);
}

int main() {
    // Context *ctx;
    // ctx = (Context *)malloc(sizeof(Context));
    // if (ctx == NULL) {
    //     perror("malloc");
    //     exit(EXIT_FAILURE);
    // }

    printf("Server PID: %d\n", getpid());

    // printf("\nInitialisation.\n");
    // ctx->bit_count = 0;
    // ctx->current_byte = 0;
    // ctx->byte_count = 0;
    // memset(ctx->utf8_char, 0, 4);
    // printf("\nInitialisation terminée.\n");

    struct sigaction sa1;

    sa1.sa_sigaction = handle_bit;
    sa1.sa_flags = SA_SIGINFO;
    sigemptyset(&sa1.sa_mask);
    if (sigaction(SIGUSR1, &sa1, NULL) == -1
        || sigaction(SIGUSR2, &sa1, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    while (1) {
        pause();  // Attendre un signal
    }

    // free(ctx);
    return 0;
}
