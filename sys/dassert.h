#pragma once

#ifdef __cplusplus
#include "logging.hpp"

#define DASSERT(f, ...)         \
    do {                        \
        if (!(f)) {             \
            LOG_D(__VA_ARGS__); \
            while (true)        \
                ;               \
        }                       \
    } while (0);

#else
/*
#include <stdbool.h>
inline void DASSERT(bool f, ...)
{
    if (!f)s
        while (true)
            ;
}
*/
#endif
