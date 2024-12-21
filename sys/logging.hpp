#pragma once

// Forward declaration
extern "C" void log_print(const char* fmt, ...);

#define LOG_D(fmt, ...)                         \
    do {                                        \
        if (1)                                  \
            log_print(fmt "\n", ##__VA_ARGS__); \
    } while (0)
