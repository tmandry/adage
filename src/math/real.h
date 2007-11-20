// real.h
//  Provides functions for working with real numbers

#ifndef REAL_H_
#define REAL_H_

#include <cmath>

#include "math/const.h"

namespace Math {

// Tests if two floating-point values are equal with an epsilon
inline bool isEqual(const double a, const double b)
{
     if (fabs (a - b) < epsilon)
          return true;
     else
          return false;
}

inline bool isEqual(const float a, const float b)
{
     if (fabs (a - b) < (float)epsilon)
          return true;
     else
          return false;
}


// Tests if a floating-point value is equal to zero with an epsilon
inline bool isZero(const double val)
{
     return (-minDouble < val) && (val < minDouble);
}

inline bool isZero(const float val)
{
     return (-minFloat < val) && (val < minFloat);
}


inline double round(const double val)
{
    if ((int)val % 10 < 5)
        return floor(val);
    else
        return ceil(val);
}


inline double floorTo(const double val, const double granularity)
{
    return floor(val / granularity) * granularity;
}

inline double ceilTo(const double val, const double granularity)
{
    return ceil(val / granularity) * granularity;
}

inline double roundTo(const double val, const double granularity)
{
    return round(val / granularity) * granularity;
}

} // namespace Math

#endif /*REAL_H_*/
