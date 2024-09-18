#ifndef CAPI_CONFIGURE_H
# define CAPI_CONFIGURE_H

//~ for accept4()
# define _GNU_SOURCE

//~ backlog value, max. count of incomplete connections
# define CAPILISTEN_BACKLOG 5

//~ allowed port range for CAPI server socket
# define CAPI_MIN_PORT 1024
# define CAPI_MAX_PORT 65000

# ifdef CAPI_DEBUG

typedef enum E_CAPI_FILETYPE {
    FILE_UNKNOWN = 0,           /* unknown */
    FILE_FIFO = 1,              /* pipe */
    FILE_CHR = 2,               /* character device */
    FILE_DIR = 4,               /* directory */
    FILE_BLK = 6,               /* block */
    FILE_REG = 8,               /* regular */
    FILE_LNK = 10,              /* link */
    FILE_SOCK = 12,             /* socket */
    FILE_WHT = 14               /* whiteout */
} T_CAPI_FILETYPE;

#  define CAPI_ENDPOINTS_DIR "endpoints"
#  define DOT_SYMBOL ("./" CAPI_ENDPOINTS_DIR "/")

const char *get_file_errormsg(T_CAPI_FILETYPE ftype);
int is_valid_path(const char *path);
char **get_endpoints(const char *root_path);
void free_endpoints(char **lst);

# endif /* CAPI_DEBUG */

#endif /* CAPI_CONFIGURE_H */
