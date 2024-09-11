#ifndef CAPI_H
# define CAPI_H

# include "configure.h"

# include <sys/socket.h>        /* socket, listen */
# include <netinet/in.h>        /* struct sockaddr_in */
# include <arpa/inet.h>         /* htons */
# include <stdarg.h>            /* utils_vaput */
# include <unistd.h>            /* write */
# include <stdlib.h>            /* exit */
# include "utils.h"
# include "checkerror.h"
# include "httpheaders.h"

/*
    Client                                   Server
 x-----------x                            x-----------x
 |getaddrinfo|                            |getaddrinfo|
 x-----------x                            x-----------x
       |                                        |
       |                                        |
       V                                        V
 x-----------x                            x-----------x
 |   socket  |                            |   socket  |
 x-----------x                            x-----------x
       |                                        |
       |                                        |
       |                                        V
       |                                  x-----------x
       |                                  |    bind   |
       |                                  x-----------x
       |                                        |
       |                                        |
       |                                        V
       |                                  x-----------x
       |                                  |   listen  |
       |                                  x-----------x
       |                                        |
       |                                        |
       V              Connection                V
 x-----------x          Request           x-----------x
 |  connect  | -   -   -   -   -   -   -> |   accept  |  <---+
 x-----------x                            x-----------x      |
       |                                        |            |
       |                                        |            |
       V                                        V            |
 x-----------x                            x-----------x      |
 |    send   |  ----------------------->  |recv ~ read|      |
 x-----------x                            x-----------x      |
       |                                        |            |
       |                                        |            |
       V                                        V            | Await
 x-----------x                            x-----------x      | connection
 |recv ~ read|  <-----------------------  |    send   |      | request from
 x-----------x                            x-----------x      | next client
       |                                        |            |
       |                                        |            |
       V                                        V            |
 x-----------x             EOF            x-----------x      |
 |   close   | -   -   -   -   -   -   -> |recv ~ read|      |
 x-----------x                            x-----------x      |
                                                |            |
                                                |            |
                                                V            |
                                          x-----------x      |
                                          |   close   |  |---+
                                          x-----------x
*/

void bind_address(int sockfd, char *address, int port);
char *recv_data(int sockfd, size_t n);
void run_server(int serverfd);
void send_data(int sockfd, char *value);

#endif /* CAPI_H */
