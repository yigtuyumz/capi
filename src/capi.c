#include "capi.h"
#include <signal.h>             // NOLINT llvmlibc-restrict-system-libc-headers

#ifdef CAPI_DEBUG
const char *
json_getfrom(const char *json_file)
{
    json_object *root = json_object_from_file(json_file);

    if (root == NULL) {
        return (NULL);
    }

    const char *val = json_object_to_json_string(root);

    return (val);
}
#endif /* CAPI_DEBUG */

void
bind_address(int sockfd, const char *bind_addr, int port)
{
    struct sockaddr_in sock_address;

    // socket type is internet
    sock_address.sin_family = AF_INET;
    // convert port number to network byte order
    sock_address.sin_port = htons(port);
    // convert ipv4str to network byte order
    check_error((inet_pton(AF_INET, bind_addr, &sock_address.sin_addr) != 1),
                "Invalid address to bind.");
    // bind adjusted socket to the address
    int bnd = bind(sockfd, (struct sockaddr *) &sock_address,
                   sizeof(struct sockaddr_in));

    check_error((bnd != 0), "Bind failed.");
}

char *
recv_data(int sockfd, size_t n)
{
    char *data_buffer = (char *) malloc(n);

    check_error((data_buffer == NULL), "Malloc recv_data fails!");

    utils_bzero((void *) data_buffer, n);
    ssize_t recv_count = recv(sockfd, (void *) data_buffer, n, 0);

    check_error((recv_count < 0), "Recv from socket failed.");

    return (data_buffer);
}

////////////////////////////////////////////////////////////////////////////////
void
capi_sigact(int signum, siginfo_t *info __attribute__((unused)), void *context __attribute__((unused))) // NOLINT misc-unused-parameters
{
    // TODO capi.c > get state of the program and take a relative action.
    if (signum == SIGINT) {     // Ctrl+C
        printf("\rRecieved SIGINT; Bye!\n");
    } else if (signum == SIGTERM) {     // kill process
        printf("\n\nByexxx!\n");
        (void) fflush(NULL);
    } else {                    // segfault etc.
        printf("%d\n", signum);
    }
}

void
//1 capi_signals(void)
capi_signals(int siglist[])
{
    struct sigaction sigact;

    sigact.sa_sigaction = capi_sigact;  // set capi signal action function as a callback
    sigemptyset(&sigact.sa_mask);       // don't block extra signals.
    sigact.sa_flags = SA_SIGINFO;       // activate siginfo_t style

    //~ activating specific signals
    //1 check_error((sigaction(SIGINT, &sigact, NULL) < 0), "Sigaction <SIGINT> failed.");
    //1 check_error((sigaction(SIGTERM, &sigact, NULL) < 0), "Sigaction <SIGTERM> failed.");
    //1 check_error((sigaction(SIGSEGV, &sigact, NULL) < 0), "Sigaction <SIGSEGV> failed.");

    while (*siglist) {
        check_error((sigaction(*siglist, &sigact, NULL) < 0),
                    "Sigaction failed.");
        printf("%d activated.\n", *siglist);
        siglist++;
    }
}

////////////////////////////////////////////////////////////////////////////////

void
run_server(int serverfd)
{
    int signal_list[] = { SIGINT, SIGTERM, SIGSEGV, 0};
    capi_signals(signal_list);

    while (1) {
        struct sockaddr_in acceptfd_addr;
        size_t acceptfd_addrlen = sizeof(struct sockaddr_in);

        // TODO capi.c > accept4 => non-blocking behaviour in the future (_GNU_SOURCE)
#ifdef _GNU_SOURCE
        printf("_GNU_SOURCE defined.\n\n");
        int acceptfd = accept4(serverfd, (struct sockaddr *) &acceptfd_addr,
                               (socklen_t *) & acceptfd_addrlen, 0);
#else
        int acceptfd = accept(serverfd, (struct sockaddr *) &acceptfd_addr,
                              (socklen_t *) & acceptfd_addrlen);
#endif /* _GNU_SOURCE */

//! Ctrl+C atildiginda program bu state'a geliyor!!!
        check_error((acceptfd < 0), "Accept failed.");
//! Ctrl+C atildiginda program bu state'a geliyor!!!


        struct linger acceptfd_linger;

        acceptfd_linger.l_onoff = 0;
        acceptfd_linger.l_linger = 0;
        int acceptfd_setopt = setsockopt(acceptfd, SOL_SOCKET,
                                         SO_LINGER, &acceptfd_linger,
                                         sizeof(struct linger));

        check_error((acceptfd_setopt < 0), "Accept fd setopt linger failed.");

#ifdef CAPI_DEBUG
        // NOLINTBEGIN -cppcoreguidelines-avoid-magic-numbers -hicpp-signed-bitwise
        utils_vaput(STDOUT_FILENO, "accepted from %d.%d.%d.%d:%d\n",
                    acceptfd_addr.sin_addr.s_addr & 0x000000FF,
                    (acceptfd_addr.sin_addr.s_addr & 0x0000FF00) >> 8,
                    (acceptfd_addr.sin_addr.s_addr & 0x00FF0000) >> 16,
                    (acceptfd_addr.sin_addr.s_addr & 0xFF000000) >> 24,
                    acceptfd_addr.sin_port & 0x0000FFFF);
        // NOLINTEND -cppcoreguidelines-avoid-magic-numbers -hicpp-signed-bitwise
#endif /* CAPI_DEBUG */

        // TODO capi.c > run_server : dynamic buffer size for accepted socket
        size_t accept_buff_sz = 1000;   // NOLINT -cppcoreguidelines-avoid-magic-numbers
        char *accepted_data = recv_data(acceptfd, accept_buff_sz);

        // TODO capi.c > accepted_data parse information from string. acquire METHOD and endpoint.
#ifdef CAPI_DEBUG
        utils_vaput(STDOUT_FILENO, "%s\n", accepted_data);
#endif /* CAPI_DEBUG */

        free(accepted_data);

        struct http_header_s *header = init_http_header();

        check_error((header == NULL), "HTTP header initialization failed.");

        // TODO capi.c > run_server : dynamic endpoints
        push_http_header(header, "HTTP/1.1 200 OK");
        push_http_header(header, "Access-Control-Allow-Origin: *");
        push_http_header(header,
                         "Access-Control-Allow-Methods: GET, POST, PUT, DELETE");
        push_http_header(header,
                         "Access-Control-Allow-Headers: Origin, X-Requested-With, Content-Type, Accept");
        push_http_header(header, "Content-Type: application/json");
        // TODO capi.c > push_http_header : dynamic Content-Length
        push_http_header(header, "Content-Length: 1112\r\n");

        // TODO capi.c > run_server : read a JSON file for each endpoint
#ifdef CAPI_DEBUG
        const char *val = json_getfrom("boom.json");

        if (val == NULL) {
            push_http_header(header, "{\"response\": \"NULL\"}");
        } else {
            if (utils_strlen(val) >= 1000) {    // NOLINT -cppcoreguidelines-avoid-magic-numbers
                push_http_header(header, "{\"response\": \"TOO BIG\"}");
            } else {
                push_http_header(header, val);
            }
        }
        free((void *) val);
#else
        push_http_header(header,
                         "{\"build\":\"RELEASE\",\"response\": \"STATIC\"}");
#endif /* CAPI_DEBUG */

        const char *header_str = get_http_header_str(header);

        send_data(acceptfd, header_str);

        free((void *) header_str);
        free_http_header(header);

        int close_acceptfd = close(acceptfd);

        check_error((close_acceptfd != 0),
                    "Error while closing accepted socket.");
    }
}

void
send_data(int sockfd, const char *value)
{
    ssize_t send_value = send(sockfd, value, utils_strlen(value), 0);

    check_error((send_value < 0), "Send to socket failed.");
}

void
close_server(int serverfd)
{
    int close_CAPI_SOCKFD = close(serverfd);

    check_error((close_CAPI_SOCKFD != 0), "CAPI_SOCKFD close failed.");
}

int
create_server(const char *bind_addr, int port)
{
    int serverfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    check_error((serverfd < 0), "CAPI_SOCKFD create failed.");

    bind_address(serverfd, bind_addr, port);

    return (serverfd);
}

void
listen_server(int serverfd)
{
    int listen_CAPI = listen(serverfd, CAPILISTEN_BACKLOG);

    check_error((listen_CAPI < 0), "listen failed.");
}

int
set_port(const char *portstr)
{
    check_error((portstr == NULL), "argv[1] is NULL.");

    int port = utils_atoi(portstr);

    check_error((port < 1024 || port > 65000),
                "Port 1024-65000 allowed.");

    return (port);
}
