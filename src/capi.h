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

# ifdef CAPI_DEBUG
#  include "json-c/json.h"
# endif /* CAPI_DEBUG */

/*
                    x----------- x        x-|DEBUG|--- x                        
                    | lib  utils | -----> | lib json-c |                        
                    x------------x        x------------x                        
                                                ||                              
                                                VV                              
    Client                                    Server                            
 x-----------x                            x------------x                        
 |getaddrinfo|                            |getaddrinfo |                        
 x-----------x                            x------------x                        
       |                                        |                               
       |                                        |                               
       V                                        V                               
 x-----------x                            x------------x                        
 |   socket  |                            |   socket   |                        
 x-----------x                            x------------x                        
       |                                        |                               
       |                                        |                               
       |                                        V                               
       |                                  x------------x                        
       |                                  |    bind    |                        
       |                                  x------------x                        
       |                                        |                               
       |                                        |                               
       |                                        V                               
       |                                  x------------x                        
       |                                  |   listen   |                        
       |                                  x------------x                        
       |                                        |                               
       |                                        |                               
       V              Connection                V                               
 x-----------x          Request           x------------x                        
 |  connect  | -   -   -   -   -   -   -> |   accept   | <--------------+       
 x-----------x                            x------------x                |       
       |                                        |                       |       
       |                                        |                       |       
       V                                        V                       |       
 x-----------x                            x------------x                |       
 |    send   |  ----------------------->  |recv ~ read |                |       
 x-----------x                            x------------x                |       
       |                                        |                       |       
       |                                        |               .--------------.
       V                                        V               | Await        |
 x-----------x                            x------------x        | connection   |
 |recv ~ read|  <-----------------------  |    send    |        | request from |
 x-----------x                            x------------x        | next client  |
       |                                        |               .--------------.
       |                                        |                       |       
       V                                        V                       |       
 x-----------x             EOF            x------------x                |       
 |   close   | -   -   -   -   -   -   -> |recv ~ read |                |       
 x-----------x                            x------------x                |       
                                                |                       |       
                                                |                       |       
                                                V                       |       
                                          x------------x                |       
                                          |   close    |  |-------------+       
                                          x------------x                        
*/

void bind_address(int sockfd, char *address, int port);
char *recv_data(int sockfd, size_t n);
void run_server(int serverfd);
void send_data(int sockfd, const char *value);

# ifdef CAPI_DEBUG
const char *json_getfrom(const char *json_file);
# endif /* CAPI_DEBUG */

#endif /* CAPI_H */
