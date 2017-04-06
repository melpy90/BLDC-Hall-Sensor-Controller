#include "RateLimiter.h"

RateLimiter::RateLimiter () {
    setLimits(1, -1, 0, 10e-3);
}

RateLimiter::RateLimiter (float R, float F, float y0, float T_d) {
    setLimits(R, F, y0, T_d);
}

void RateLimiter::reset() {
    y0 = 0;
}

void RateLimiter::setLimits(float R, float F) {
    if (R < 0) {
        R = -R;
    }
    if (F > 0) {
        F = -F;
    }
    this->R = R;
    this->F = F;
}
 
void RateLimiter::setLimits(float R, float F, float y0, float T_d) {
    if (R < 0) {
        R = -R;
    }
    if (F > 0) {
        F = -F;
    }
    if (T_d <= 1e-6) {
        T_d = 1e-3;
    }
    this->T_d = T_d;
    this->y0 = y0;
    this->R = R;
    this->F = F;
}
 
float RateLimiter::out (float u) {
    rate = (u - y0) / T_d;
    if (rate > R) {
        y = R * T_d + y0;
    } else if (rate < F) {
        y = F * T_d + y0;
    } else {
        y = u;
    }
    y0 = y;
    return y;
}