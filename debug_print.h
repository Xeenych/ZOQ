#ifdef DEBUG_PRINT
#include <stdio.h>
#define MYDEBUG 1
#else
#define MYDEBUG 0
#endif

#define debug_print(...) \
do { if (MYDEBUG) printf(__VA_ARGS__); } while (0)
