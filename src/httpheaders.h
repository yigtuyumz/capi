#ifndef HTTP_HEADERS_H
# define HTTP_HEADERS_H

# include "utils.h"
# include <stdlib.h>            /* exit */

struct http_header_s {
    char **fields;
    __uint32_t sz;
};

struct http_header_s *init_http_header(void);

/* concats (field + '\r\n') and pushes */
size_t push_http_header(struct http_header_s *header, const char *field);

/* no concat, direct push field value. */
size_t push_http_header2(struct http_header_s *header, const char *field);

const char *get_http_header_str(struct http_header_s *header);

void free_http_header(struct http_header_s *header);

#endif /* HTTP_HEADERS_H */
