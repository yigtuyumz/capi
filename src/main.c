#include "capi.h"

// TODO main.c > put in common values into a struct. (System Hardening)
int
main(int argc __attribute__((unused)), char *argv[])
{
#ifdef CAPI_DEBUG
    check_error((argv[2] == NULL), "argv[2] is NULL.");
#endif /* CAPI_DEBUG */

    check_error((argv[1] == NULL), "argv[1] is NULL.");

    int CAPI_PORT = utils_atoi(argv[1]);

    check_error((CAPI_PORT < 1024 || CAPI_PORT > 65000),
                "Port 1024-65000 allowed.");

    int CAPI_SOCKFD = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    check_error((CAPI_SOCKFD < 0), "CAPI_SOCKFD create failed.");

    bind_address(CAPI_SOCKFD, CAPI_BIND_ADDR, CAPI_PORT);

    int listen_CAPI = listen(CAPI_SOCKFD, CAPILISTEN_BACKLOG);

    check_error((listen_CAPI < 0), "listen failed.");

    run_server(CAPI_SOCKFD);

    // TODO main.c > closing CAPI socket -> signal sensitive (SIGINT etc.)
    int close_CAPI_SOCKFD = close(CAPI_SOCKFD);

    check_error((close_CAPI_SOCKFD != 0), "CAPI_SOCKFD close failed.");

    return (0);
}
