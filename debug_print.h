#ifdef DEBUG_PRINT
#include <stdio.h>
#define DEBUG 1
#else
#define DEBUG 0
#endif

#define debug_print(...) \
do { if (DEBUG) printf(__VA_ARGS__); } while (0)