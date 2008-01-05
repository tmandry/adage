#ifndef CONVERT_H_
#define CONVERT_H_

#include "const.h"

namespace Math {

inline double toDegrees(double radians) {return radians * 180 / PI;}
inline double toRadians(double degrees) {return degrees * PI / 180;}

} //namespace Math

#endif /*CONVERT_H_*/
