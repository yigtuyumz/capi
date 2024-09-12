#include "capi.h"

int
main(int argc __attribute__((unused)), char *argv[])
{
    check_error((argv[1] != NULL), "argv[1] is NULL.");

    int CAPI_PORT = utils_atoi(argv[1]);

    check_error((1024 <= CAPI_PORT && CAPI_PORT <= 65000),
                "Port 1024-65000 allowed.");

    int CAPI_SOCKFD = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    check_error((CAPI_SOCKFD >= 0), "CAPI_SOCKFD create failed.");

    bind_address(CAPI_SOCKFD, "127.0.0.1", CAPI_PORT);

    int listen_CAPI = listen(CAPI_SOCKFD, CAPILISTEN_BACKLOG);

    check_error((listen_CAPI >= 0), "listen failed.");

    while (1) {
        run_server(CAPI_SOCKFD);
    }

    int close_CAPI_SOCKFD = close(CAPI_SOCKFD);

    check_error((close_CAPI_SOCKFD == 0), "CAPI_SOCKFD close failed.");

    return (0);
}
