#ifndef __P2DEFS_H__
#define __P2DEFS_H__

#include <cstdio>
#include <cstdint>

//  NULL just in case ----------------------

#ifdef NULL
#undef NULL
#endif
#define NULL 0

// Deletes a buffer
#define RELEASE(x)          \
    {                       \
        if (x != nullptr) { \
            delete x;       \
            x = nullptr;    \
        }                   \
    }

// Deletes an array of buffers
#define RELEASE_ARRAY(x)    \
    {                       \
        if (x != nullptr) { \
            delete[] x;     \
            x = nullptr;    \
        }                   \
    }

#define IN_RANGE(value, min, max) (((value) >= (min) && (value) <= (max)) ? 1 : 0)
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define TO_BOOL(a) ((a != 0) ? true : false)

typedef uint32_t uint;
typedef uint32_t uint32;
typedef uint64_t uint64;
typedef uint8_t uchar;

template <class VALUE_TYPE>
void SWAP(VALUE_TYPE &a, VALUE_TYPE &b)
{
    VALUE_TYPE tmp = a;
    a = b;
    b = tmp;
}

// Standard string size
#define SHORT_STR 32
#define MID_STR 255
#define HUGE_STR 8192

// Joins a path and file
inline const char *PATH(const char *folder, const char *file)
{
    static char path[MID_STR];
    snprintf(path, MID_STR, "%s/%s", folder, file);
    return path;
}

// Performance macros
#define PERF_START(timer) timer.Start()
#define PERF_PEEK(timer) LOG("%s took %f ms", __FUNCTION__, timer.ReadMs())

//Angles
#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f

template <typename T>
T clamp(const T &n, const T &lower, const T &upper)
{
    return MAX(lower, MIN(n, upper));
}

#endif
