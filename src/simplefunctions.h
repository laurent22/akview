#ifndef MV_SIMPLEFUNCTIONS_H
#define MV_SIMPLEFUNCTIONS_H

#include <cmath>

template <class T>
T round (T r) {
    return (r > 0.0) ? floor(r + 0.5) : ceil(r - 0.5);
}

#endif