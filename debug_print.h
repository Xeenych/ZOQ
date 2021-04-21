#include <stdio.h>
#ifdef DEBUG_PRINT
#define MYDEBUG 1
#else
#define MYDEBUG 0
#endif

#define debug_print(...) \
do { if (MYDEBUG) printf(__VA_ARGS__); } while (0)
