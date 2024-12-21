#pragma once
#include "log_print.hpp"

#define LOG_D(fmt, ...)                         \
    do {                                        \
        if (1)                                  \
            log_print(fmt "\n", ##__VA_ARGS__); \
    } while (0)
