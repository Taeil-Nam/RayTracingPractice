#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <limits>
#include <memory>


// Usings
using std::shared_ptr;	// std:: 생략
using std::make_shared; // std:: 생략
using std::sqrt;		// std:: 생략

// Constants
const double infinity = std::numeric_limits<double>::infinity();	// 무한대
const double pi = 3.1415926535897932385;	// pi

// Utility Functions
inline double degrees_to_radians(double degrees) { // degree -> radian 변환
    return degrees * pi / 180.0;
}

inline double random_double() { // 실수 생성 함수
    // Returns a random real in [0,1).
    return rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max) {
    // Returns a random real in [min,max).
    return min + (max-min)*random_double();
}

// Common Headers

#include "ray.h"
#include "vec3.h"

#endif