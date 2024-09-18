#include "httpheaders.h"

void
free_http_header(struct http_header_s *header)
{
    if (header == NULL) {
        return;
    }

    __uint32_t header_pos = 0;

    while (header_pos < header->sz) {
        free(*(header->fields + header_pos));
        header_pos++;
    }

    free((void *) header->fields);
    free(header);
}

const char *
get_http_header_str(struct http_header_s *header)
{
    //~ header->sz comparison is extra
    if (header == NULL || header->fields == NULL || header->sz == 0) {
        return (NULL);
    }

    size_t tot_len = 1;

    __uint32_t field_offset = 0;

    while (field_offset < header->sz) {
        tot_len += utils_strlen(*(header->fields + field_offset));
        field_offset++;
    }

    //~ extra byte for NULL terminator
    char *ret_str = (char *) malloc(sizeof(char) * tot_len);

    if (ret_str == NULL) {
        return (NULL);
    }

    *ret_str = 0;
    field_offset = 0;

    while (field_offset < header->sz) {
        utils_strcat(ret_str, *(header->fields + field_offset));
        field_offset++;
    }

    return ((const char *) ret_str);
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
push_http_header(struct http_header_s *header, const char *field)
{
    if (field == NULL || header == NULL) {
        return (0);
    }

    char **temp =
        (char **) realloc((void *) header->fields,
                          sizeof(char *) * (header->sz + 1));

    if (temp == NULL) {
        return (0);
    }

    size_t copy_bytes = sizeof(char) * (utils_strlen(field) + 3);
    char *field_copy = (char *) malloc(copy_bytes);

    header->fields = temp;

    if (field_copy == NULL) {
        return (0);
    }

    utils_strcpy(field_copy, field);
    utils_strcat(field_copy, "\r\n");

    *(header->fields + header->sz) = field_copy;
    header->sz++;

    return (copy_bytes);
}

size_t
push_http_header2(struct http_header_s *header, const char *field)
{
    if (field == NULL || header == NULL) {
        return (0);
    }

    char **temp =
        (char **) realloc((void *) header->fields,
                          sizeof(char *) * (header->sz + 1));

    if (temp == NULL) {
        return (0);
    }

    size_t copy_bytes = sizeof(char) * (utils_strlen(field) + 1);
    char *field_copy = (char *) malloc(copy_bytes);

    header->fields = temp;

    if (field_copy == NULL) {
        return (0);
    }

    utils_strcpy(field_copy, field);

    *(header->fields + header->sz) = field_copy;
    header->sz++;

    return (copy_bytes);
}
