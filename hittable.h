#ifndef HITTABLE_H
#define HITTABLE_H

#include "rtweekend.h"

// 클래스 전방선언
class material;

// ray hit시 t, P, 법선 벡터를 저장할 구조체
struct hit_record {
    point3 p;
    vec3 normal;
    shared_ptr<material> mat_ptr;
    double t;
    double u;
    double v;
    bool front_face;

    inline void set_face_normal(const ray& r, const vec3& outward_normal) { // ray와 표면의 외부로 나가는 법선을 받음
        front_face = dot(r.direction(), outward_normal) < 0; // ray와 법선의 내적 값이 음수라면 front_face = true
        normal = front_face ? outward_normal :-outward_normal; 
        // front_face = true인 경우, 표면의 외부로 나가는 법선을 사용
        // front_face = false인 경우, 표면의 내부로 나가는 법선을 사용
    }
};

// ray가 hit 했는지 확인하기 위한 hit 함수를 가지고 있는 추상 클래스
class hittable {
    public:
        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};

#endif