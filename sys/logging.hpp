#pragma once
#include "log_print.hpp"

#ifdef LOG_ENABLE
#define LOG_D(fmt, ...)                                             \
    do {                                                            \
        if (LOG_ENABLE)                                             \
            log_print("%-30s: " fmt "\n", __FILE__, ##__VA_ARGS__); \
    } while (0)
#else
#define LOG_D(fmt, ...)
#endif