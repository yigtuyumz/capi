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
bind_address(int sockfd, char *address, int port)
{
    struct sockaddr_in sock_address;

    sock_address.sin_family = AF_INET;
    sock_address.sin_port = htons(port);
    check_error((inet_pton(AF_INET, address, &sock_address.sin_addr) != 1),
                "Invalid address to bind.");
    int bnd = bind(sockfd, (struct sockaddr *) &sock_address,
                   sizeof(struct sockaddr_in));

    check_error((bnd != 0), "Bind failed.");
}

char *
recv_data(int sockfd, size_t n)
{
    char *data = (char *) malloc(n);

    check_error((data == NULL), "Malloc recv_data fails!");

    utils_bzero(data, n);
    ssize_t recv_count = recv(sockfd, data, n, 0);

    check_error((recv_count < 0), "Recv from socket failed.");

    return (data);
}

////////////////////////////////////////////////////////////////////////////////
// void(*) (int, siginfo_t *, void *)
void
capi_sigact(int signum, siginfo_t *info, void *context) // NOLINT misc-unused-parameters
{
    if (signum == SIGINT) {     // Ctrl+C
        printf("\rBye!\n");
    } else if (signum == SIGTERM) {     // kill process
        printf("\n\nByexxx!\n");
        (void) fflush(NULL);
    } else {                    // segfault etc.
        printf("%d\n", signum);
    }
}

void
capi_signals(void)
{
    struct sigaction sigact;

    sigact.sa_sigaction = capi_sigact;
    sigemptyset(&sigact.sa_mask);       // don't block extra signals.
    sigact.sa_flags = SA_SIGINFO;       // sorry ;)

    int interrupt_act = sigaction(SIGINT, &sigact, NULL);

    check_error((interrupt_act == -1), "Sigaction <SIGINT> failed.");

    int term_act = sigaction(SIGTERM, &sigact, NULL);

    check_error((term_act == -1), "Sigaction <SIGTERM> failed.");

    int segfault_act = sigaction(SIGSEGV, &sigact, NULL);

    check_error((segfault_act == -1), "Sigaction <SIGSEGV> failed.");
}
////////////////////////////////////////////////////////////////////////////////

void
run_server(int serverfd)
{
    // capi_signals();
    while (1) {
        struct sockaddr_in acceptfd_addr;
        size_t acceptfd_addrlen = sizeof(struct sockaddr_in);

        // TODO capi.c > accept4 => non-blocking behaviour in the future (_GNU_SOURCE)
#ifdef _GNU_SOURCE
        int acceptfd = accept4(serverfd, (struct sockaddr *) &acceptfd_addr,
                               (socklen_t *) & acceptfd_addrlen, 0);
#else
        int acceptfd = accept(serverfd, (struct sockaddr *) &acceptfd_addr,
                              (socklen_t *) & acceptfd_addrlen);
#endif /* _GNU_SOURCE */

        check_error((acceptfd < 0), "Accept failed.");

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
    // char *send_data_buffer = (char *) malloc(sizeof(char) * n);

    // utils_bzero(send_data_buffer, n);
    // utils_strcpy(send_data_buffer, value);
    ssize_t send_value = send(sockfd, value, utils_strlen(value), 0);

    check_error((send_value < 0), "Send to socket failed.");

}
