#ifndef OVRTOS_HPP_
#define OVRTOS_HPP_

#include "OVQueue.hpp"
#include "OVThread.hpp"

typedef enum {
    rtosPriorityIdle = -3,        /* idle (lowest) */
    rtosPriorityLow = -2,         /* (low) */
    rtosPriorityBelowNormal = -1, /* below normal */
    rtosPriorityNormal = 0,       /* normal (default) */
    rtosPriorityAboveNormal = +1, /* above normal */
    rtosPriorityHigh = +2,        /* high */
    rtosPriorityRealtime = +3,    /* realtime (highest) */
    rtosPriorityError = 0x84      /* system cannot determine priority or thread has illegal priority */
} RTOSPriority_T;

#endif