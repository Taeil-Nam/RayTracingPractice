#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable { // 추상 클래스 hittable 상속 받음
    public:
        sphere() {} // 기본 생성자
        sphere(point3 cen, double r) : center(cen), radius(r) {}; // 구의 중심, 반지름을 설정하는 생성자

		// 가상 함수 hit (추상 클래스 hittable로 부터 물려 받은 함수)
        virtual bool hit(
            const ray& r, double t_min, double t_max, hit_record& rec) const override;

    public:
        point3 center; // 구의 중심
        double radius; // 반지름
};

/* 구에서 t에 대한 이차 방정식 */
// t^2 * (b dot b) + t * 2 * (b dot (A - C)) + ((A - C) dot (A - C) - r^2) = 0
bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    vec3 oc = r.origin() - center; // A - C 벡터
    auto a = r.direction().length_squared(); // a항 (b dot b = b 길이의 제곱)
    auto half_b = dot(oc, r.direction()); // b항 (간편식)
    auto c = oc.length_squared() - radius*radius; // c항 (oc dot oc = oc 길이의 제곱)

    auto discriminant = half_b*half_b - a*c; // 간편식의 판별식
    if (discriminant < 0) return false; // 근(root)이 없으므로 ray가 구에 hit 하지 않음
    auto sqrtd = sqrt(discriminant); // 판별식을 루트 씌운 값

    // Find the nearest root that lies in the acceptable range.
	// 근이 있을 수 있는 범위에서 가장 가까운 근을 찾기
    auto root = (-half_b - sqrtd) / a; // root = 음수 근
    if (root < t_min || t_max < root) { // 근이 있을 수 있는 범위가 아닌 경우
        root = (-half_b + sqrtd) / a; // root = 양수 근
        if (root < t_min || t_max < root) // 근이 있을 수 있는 범위가 아닌 경우
            return false; // 근이 없음
    }

	// 근이 있는 경우
    rec.t = root; // 현재의 근 저장
    rec.p = r.at(rec.t); // ray가 구에 hit한 점 P 저장
	vec3 outward_normal = (rec.p - center) / radius; // 점 P에 대한 구 표면의 외부로 나가는 법선 벡터 저장
    rec.set_face_normal(r, outward_normal);

    return true;
}

#endif