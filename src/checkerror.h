#ifndef CHECK_ERROR_H
# define CHECK_ERROR_H

# include "utils.h"
# include <stdlib.h>            /* exit */

# define check_error(a, b) \
    _Generic((a), \
        char *: _check_error_str, \
        const char *: _check_error_str, \
        default: _check_error_int \
    )(a, b)

//~ put error messages to the 'err_msg' in case an error occurs.
void
  _check_error_int(int boolean, char *err_msg);

void
  _check_error_str(const char *boolean, char *err_msg);

#endif /* CHECK_ERROR_H */
