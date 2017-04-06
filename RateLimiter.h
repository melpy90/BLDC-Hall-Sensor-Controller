#ifndef RATE_LIMITER_H
#define RATE_LIMITER_H

#include "mbed.h"

/** A rate limiter class.
 * 
 * Author(s): TVZ Mechatronics Team
 *
 */
class RateLimiter {
    public:
        /** Default constructor */
        RateLimiter ();
        /** Constructor receives rising (R) and falling (F) rate limits,
         * initial condition of the output and sample time in seconds. */
        RateLimiter (float R, float F, float initialCondition, float sampleTime);
        /** Setting the rate limits R and F. */
        void setLimits (float R, float F);
        void setLimits (float R, float F, float initialCondition, float sampleTime);
        /** Calculating the output of the rate limiter for the given input. */
        float out (float input);
        /** Reset the output of the rate limiter to zero. */
        void reset();
    private:
        float R, F, y0, y, T_d, rate;
};

#endif // RATE_LIMITER_H