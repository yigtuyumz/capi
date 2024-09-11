#include "capi.h"

void
bind_address(int sockfd, char *address, int port)
{
    struct sockaddr_in sock_address;

    sock_address.sin_family = AF_INET;
    sock_address.sin_port = htons(port);
    check_error(inet_pton(AF_INET, address, &sock_address.sin_addr) > 0,
                "Invalid address to bind.");
    int bnd = bind(sockfd, (struct sockaddr *) &sock_address,
                   sizeof(struct sockaddr_in));

    check_error((bnd == 0), "Bind failed.");
}

char *
recv_data(int sockfd, size_t n)
{
    char *data = (char *) malloc(n);

    check_error(data, "Malloc recv_data fails!");

    utils_bzero(data, n);
    ssize_t recv_count = recv(sockfd, data, n, 0);

    check_error((recv_count >= 0), "Recv from socket failed.");

    return (data);
}

void
run_server(int serverfd)
{
    int acceptfd;
    struct sockaddr_in acceptfd_addr;
    size_t acceptfd_addrlen = sizeof(struct sockaddr_in);

    acceptfd = accept(serverfd, (struct sockaddr *) &acceptfd_addr,
                      (socklen_t *) & acceptfd_addrlen);

    check_error((acceptfd >= 0), "Accept failed.");

    struct linger acceptfd_linger;
    int acceptfd_setopt;

    acceptfd_linger.l_onoff = 0;
    acceptfd_linger.l_linger = 0;
    acceptfd_setopt = setsockopt(acceptfd, SOL_SOCKET,
                                 SO_LINGER, &acceptfd_linger,
                                 sizeof(struct linger));

    check_error((acceptfd_setopt >= 0), "Accept fd setopt linger failed.");

    utils_vaput(STDOUT_FILENO, "accepted from %d.%d.%d.%d:%d\n",
                acceptfd_addr.sin_addr.s_addr & 0x000000FF,
                (acceptfd_addr.sin_addr.s_addr & 0x0000FF00) >> 8,
                (acceptfd_addr.sin_addr.s_addr & 0x00FF0000) >> 16,
                (acceptfd_addr.sin_addr.s_addr & 0xFF000000) >> 24,
                acceptfd_addr.sin_port & 0x0000FFFF);

    // TODO capi.c > run_server : dynamic buffer size for accepted socket
    size_t accept_buff_sz = 1000;
    char *accepted_data = recv_data(acceptfd, accept_buff_sz);

    utils_vaput(STDOUT_FILENO, "%s\n", accepted_data);

    free(accepted_data);

    struct http_header_s *header = init_http_header();

    prepare_http_header(header, "HTTP/1.1 200 OK");
    prepare_http_header(header, "Access-Control-Allow-Origin: *");
    prepare_http_header(header,
                        "Access-Control-Allow-Methods: GET, POST, PUT, DELETE");
    prepare_http_header(header,
                        "Access-Control-Allow-Headers: Origin, X-Requested-With, Content-Type, Accept");
    prepare_http_header(header, "Content-Type: application/json");
    prepare_http_header(header, "Content-Length: 1112\r\n");
    prepare_http_header(header, "{\
  \"result\": [\
    {\
      \"status\": \"active\",\
      \"name\": {\
        \"first\": \"Margarett\",\
        \"middle\": \"Noah\",\
        \"last\": \"Luettgen\"\
      },\
      \"username\": \"Margarett-Luettgen\",\
      \"password\": \"Wx2dPleO7h0BS81\",\
      \"emails\": [\
        \"Vance.Bruen@gmail.com\",\
        \"Electa82@gmail.com\"\
      ],\
      \"phoneNumber\": \"863-504-1328 x818\",\
      \"location\": {\
        \"street\": \"58931 Rhett Walks\",\
        \"city\": \"Beaulahtown\",\
        \"state\": \"Kansas\",\
        \"country\": \"Taiwan\",\
        \"zip\": \"45096\",\
        \"coordinates\": {\
          \"latitude\": 82.0592,\
          \"longitude\": -96.0192\
        }\
      },\
      \"website\": \"https://overjoyed-hornet.name/\",\
      \"domain\": \"frizzy-timber.com\",\
      \"job\": {\
        \"title\": \"Internal Mobility Specialist\",\
        \"descriptor\": \"Corporate\",\
        \"area\": \"Research\",\
        \"type\": \"Planner\",\
        \"company\": \"Shanahan - Hammes\"\
      },\
      \"creditCard\": {\
        \"number\": \"4936665241163\",\
        \"cvv\": \"376\",\
        \"issuer\": \"maestro\"\
      },\
      \"uuid\": \"fcd0abbf-ad58-493f-aeb2-fc3a711304f4\",\
      \"objectId\": \"668dcb65ec04bdd83ae85e13\"\
    }\
  ]\
}");

    //TODO capi.c > run_server : dynamic endpoints
    //TODO capi.c > run_server : read a JSON file for each endpoint

    char *header_str = get_http_header_str(header);

    send_data(acceptfd, header_str);

    free(header_str);
    free_http_header(header);

    int close_acceptfd = close(acceptfd);

    check_error((close_acceptfd == 0), "Error while closing accepted socket.");
}

void
send_data(int sockfd, char *value)
{
    // char *send_data_buffer = (char *) malloc(sizeof(char) * n);

    // utils_bzero(send_data_buffer, n);
    // utils_strcpy(send_data_buffer, value);
    ssize_t send_value = send(sockfd, value, utils_strlen(value), 0);

    check_error((send_value >= 0), "Send to socket failed.");

}
