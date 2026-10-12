#include "types.h"
#include "sdk/msl.h"

/* Double multiply of the soft-float runtime. */
double _dmul(double a, double b);

#define HUGE 1.0e300
#define TINY 1.0e-300
#define TWO_POW_54 18014398509481984.0
#define TWO_POW_MINUS_54 5.55111512312578270212e-17

/* Returns x * 2^n, built in the exponent bits. */
double ldexp(double x, int n)
{
    int k;
    int hx;
    int lx;

    if (__fpclassifyd(x) <= FP_INFINITE || x == 0.0) {
        return x;
    }

    hx = ((int *)&x)[1];
    lx = ((int *)&x)[0];
    k = (hx & 0x7FF00000) >> 20;
    if (k == 0) {
        if ((lx | (hx & 0x7FFFFFFF)) == 0) {
            return x;
        }
        /* Subnormal: normalise first. Needed to match: the cast makes the compiler reload x from the stack. */
        x = _dmul(*(double *)&x, TWO_POW_54);
        hx = ((int *)&x)[1];
        k = ((hx & 0x7FF00000) >> 20) - 54;
        if (n < -50000) {
            return _dmul(TINY, x);
        }
    }
    if (k == 0x7FF) {
        return x + x;
    }
    k = k + n;
    if (k > 0x7FE) { /* overflow */
        return _dmul(HUGE, copysign(HUGE, x));
    }
    if (k > 0) {
        ((int *)&x)[1] = (hx & 0x800FFFFF) | (k << 20);
        return x;
    }
    if (k <= -54) {
        /* n > 50000 means k + n wrapped around */
        if (n > 50000) {
            return _dmul(HUGE, copysign(HUGE, x));
        } else {
            return _dmul(TINY, copysign(TINY, x));
        }
    }
    /* subnormal result */
    k += 54;
    ((int *)&x)[1] = (hx & 0x800FFFFF) | (k << 20);
    return _dmul(TWO_POW_MINUS_54, x);
}
