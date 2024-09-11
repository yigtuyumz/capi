#include "checkerror.h"

void
_check_error_int(int boolean, char *err_msg)
{
    if (!boolean) {
        if (err_msg == NULL) {
            utils_putstr(STDERR_FILENO, "Unknown error occured.\n");
        } else {
            utils_vaput(STDERR_FILENO, "%s\n", err_msg);
        }
        // TODO checkerror.c > _check_error_int : non-static return values
        exit(1);
    }
}

void
_check_error_str(char *boolean, char *err_msg)
{
    if (!boolean) {
        if (err_msg == NULL) {
            utils_putstr(STDERR_FILENO, "Unknown error occured.\n");
        } else {
            utils_vaput(STDERR_FILENO, "%s\n", err_msg);
        }
        // TODO checkerror.c > _check_error_str : non-static return values
        exit(1);
    }
}
