
#ifndef _KOCH_COMMON_H_
#define _KOCH_COMMON_H_

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <math.h>
#include <assert.h>
#include <stdbool.h>
#include <complex.h>
#include <inttypes.h>

// defines
#define PI 3.141592653589793
#define DEGREE_TO_RADIAN(x) (((double)x * PI) / 180)
#define MAX_ARG_SIZE 0x80
#define BORDER_CALC(seg) ((3 * seg) / 12)

// Log
#define LOG_LEVEL 3
#define TRACE(__level,__message) if ( __level >= LOG_LEVEL ) { puts(__message".\n"); }
#define INFO(__message) printf("[%s: %s, l.%d] %s.\n", __FILE__, __func__, __LINE__, __message);

// Other
#define UNUSED(arg) ((void)(arg))
#define SAFE_FREE(p) if (p) { free(p); p = NULL; }

#endif
