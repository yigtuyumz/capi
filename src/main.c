#include "capi.h"


// TODO main.c > put in common values into a struct. (System Hardening)
int
main(int argc __attribute__((unused)), char *argv[])
{
#ifdef CAPI_DEBUG
    check_error((argv[2] == NULL), "argv[2] is NULL.");
#endif /* CAPI_DEBUG */

    int CAPI_PORT = set_port(argv[1]);

    int CAPI_SOCKFD = create_server(CAPI_BIND_ADDR, CAPI_PORT);

    listen_server(CAPI_SOCKFD);

    run_server(CAPI_SOCKFD);

    close_server(CAPI_SOCKFD);

    return (0);
}
