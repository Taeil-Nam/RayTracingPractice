#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"

// ray hit시 t, P, 법선 벡터를 저장할 구조체
struct hit_record {
    point3 p;
    vec3 normal;
    double t;
};

// ray가 hit 했는지 확인하기 위한 hit 함수를 가지고 있는 추상 클래스
class hittable {
    public:
        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};

#endif