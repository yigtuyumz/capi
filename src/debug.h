
/* extra debug header for manual injection in the codebase. */

/* CAPI_DEBUG can be defined while build-time with gcc -DCAPI_DEBUG=1 argument. */
#ifndef CAPI_DEBUG
# define CAPI_DEBUG
# pragma message("Debug mode is enabled.")

# include "json-c/json.h"

//~ endpoint management is provided through static json files
# define STATIC_ENDPOINTS 1

//~ name of folder in CWD which contains all .json files (aka endpoints for this project)
# define ENDPOINT_ROOT_DIR "./endpoints"

#endif /* CAPI_DEBUG */
