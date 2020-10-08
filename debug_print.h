#ifdef DEBUG_PRINT
#define DEBUG 1
#else
#define DEBUG 0
#endif

#define debug_print(...) \
do { if (DEBUG) UNUSED(printf(__VA_ARGS__)); } while (0)