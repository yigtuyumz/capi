#include "httpheaders.h"

void
free_http_header(struct http_header_s *header)
{
    if (header == NULL) {
        return;
    }

    __uint32_t sz = 0;

    while (sz < header->sz) {
        free(*(header->fields + sz));
        sz++;
    }

    free(header->fields);
    free(header);
}

char *
get_http_header_str(struct http_header_s *header)
{
    if (header == NULL || *(header->fields) == NULL || header->sz == 0) {
        return (NULL);
    }

    __uint32_t i = 0;
    size_t tot_len = 1;

    while (i < header->sz) {
        tot_len += utils_strlen(*(header->fields + i));
        i++;
    }

    i = 0;

    char *ret_str = (char *) malloc(sizeof(char) * tot_len);

    if (ret_str == NULL) {
        return (NULL);
    }

    *ret_str = 0;

    while (i < header->sz) {
        utils_strcat(ret_str, *(header->fields + i));
        i++;
    }

    return (ret_str);
}

struct http_header_s *
init_http_header(void)
{
    struct http_header_s *header = (struct http_header_s *)
        malloc(sizeof(struct http_header_s));

    if (header == NULL) {
        return (NULL);
    }

    header->sz = 0;
    header->fields = NULL;

    return (header);
}

size_t
prepare_http_header(struct http_header_s *header, char *field)
{
    if (field == NULL || header == NULL) {
        return (0);
    }

    (header->sz)++;

    header->fields = (char **) realloc(header->fields,
                                       sizeof(char *) * (header->sz));

    if (header->fields == NULL) {
        return (0);
    }
    size_t copy_bytes = sizeof(char) * (utils_strlen(field) + 3);
    char *field_copy = (char *) malloc(copy_bytes);

    if (field_copy == NULL) {
        return (0);
    }

    utils_strcpy(field_copy, field);
    utils_strcat(field_copy, "\r\n");

    *(header->fields + (header->sz - 1)) = field_copy;

    return (copy_bytes);
}

size_t
prepare_http_header2(struct http_header_s *header, char *field)
{
    if (header == NULL || field == NULL) {
        return (0);
    }

    (header->sz)++;
    header->fields = (char **) realloc(header->fields,
                                       sizeof(char *) * (header->sz));

    if (header->fields == NULL) {
        return (0);
    }

    size_t copy_bytes = (utils_strlen(field) + 1) * sizeof(char);

    *(header->fields + (header->sz - 1)) = (char *) malloc(copy_bytes);
    utils_strcpy(*(header->fields + (header->sz - 1)), field);

    return (copy_bytes);
}
