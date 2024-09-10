#include "checkerror.h"

void
_check_error_int(int boolean, char *err_msg)
{
    if (!boolean) {
        if (err_msg == NULL) {
            utils_putstr(STDOUT_FILENO, "Unknown error occured.");
        } else {
            utils_putstr(STDERR_FILENO, err_msg);
        }
        utils_putchar(STDERR_FILENO, '\n');
        exit(1);
    }
}

void
_check_error_str(char *boolean, char *err_msg)
{
    if (!boolean) {
        if (err_msg == NULL) {
            utils_putstr(STDOUT_FILENO, "Unknown error occured.");
        } else {
            utils_putstr(STDERR_FILENO, err_msg);
        }
        utils_putchar(STDERR_FILENO, '\n');
        exit(1);
    }
}
