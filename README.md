*This project has been created as part of the 42 curriculum by drhaouha.*

# Minitalk

## Description

Minitalk is a 42 project that implements a small data exchange program using exclusively UNIX signals. A `server` process receives messages sent bit by bit by a `client` process, using only `SIGUSR1` and `SIGUSR2` to encode binary data.

The core challenge is encoding each character as 8 bits and transmitting them one signal at a time, while handling signal reliability on Linux (signals of the same type are not queued when one is already pending).

## How it works

Each character of the string is broken down into 8 bits. The client sends:
- `SIGUSR1` → bit 0
- `SIGUSR2` → bit 1

The server reconstructs each byte and prints the full string once all characters are received. The server never needs to restart between messages.

## Compilation

```bash
make        # builds both server and client
make bonus  # builds server_bonus and client_bonus
make clean  # removes object files
make fclean # removes object files and binaries
make re     # fclean + make
```

## Usage

Start the server first — it prints its PID on launch:

```bash
./server
# Output: Server PID: 12345
```

Then send a message from the client:

```bash
./client <server_pid> "Hello, World!"
```

The server prints the received string immediately. It can receive messages from multiple clients in a row without restarting.

### Performance requirement

100 characters must be transmitted in under 1 second. If it seems slow, it is too slow.

## Bonus

| Feature | Description |
|---------|-------------|
| Acknowledgement | Server sends a signal back to the client after receiving each complete message |
| Unicode support | Multi-byte UTF-8 characters are transmitted and printed correctly |

## Rules

- Written in C, compiled with `cc -Wall -Wextra -Werror`
- Norm compliant
- Communication uses **only** `SIGUSR1` and `SIGUSR2`
- At most one global variable per program (client and server each), usage must be justified
- No memory leaks, no unexpected crashes
- `sigaction` preferred over `signal` for reliable signal handling

## External functions allowed

`write`, `ft_printf` (or equivalent you coded), `signal`, `sigemptyset`, `sigaddset`, `sigaction`, `kill`, `getpid`, `malloc`, `free`, `pause`, `sleep`, `usleep`, `exit`

## Resources

- [man 2 sigaction](https://man7.org/linux/man-pages/man2/sigaction.2.html)
- [man 2 kill](https://man7.org/linux/man-pages/man2/kill.2.html)
- [man 2 getpid](https://man7.org/linux/man-pages/man2/getpid.2.html)
- [UNIX signals — overview](https://man7.org/linux/man-pages/man7/signal.7.html)
- [Unicode and UTF-8 encoding](https://www.utf8.com/)
- [Bitwise operations in C](https://en.wikipedia.org/wiki/Bitwise_operations_in_C)
