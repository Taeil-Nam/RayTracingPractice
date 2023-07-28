#ifndef CAMERA_H
#define CAMERA_H

#include "rtweekend.h"

class camera {
    public:
        camera(
            point3 lookfrom,
            point3 lookat,
            vec3 vup,
            double vfov,
            double aspect_ratio)
        {
            auto theta = degrees_to_radians(vfov);  // vfov(시야각)을 radian으로 저장
            auto h = tan(theta / 2);    // height의 절반 길이인 h 계산
            auto viewport_height = 2.0 * h; // height = 2h
    		auto viewport_width = aspect_ratio * viewport_height; // 3.5
    		
            auto focal_length = 1.0; // 초점거리 (카메라와 viewport의 거리)

            auto w = unit_vector(lookfrom - lookat);
            auto u = unit_vector(cross(vup, w));
            auto v = cross(w, u);

			origin = lookfrom; // ray가 생성되는 곳 (카메라)
			horizontal = u*viewport_width;  // viewport의 x 좌표 범위
			vertical = v*viewport_height;   // viewport의 y 좌표 범위
			lower_left_corner = origin - horizontal/2 - vertical/2 - w; // viewport의 왼쪽 및 좌표
        }

        ray get_ray(double s, double t) const {
            return ray(origin, lower_left_corner + s*horizontal + t*vertical - origin);
        }

    private:
        point3 origin;
        point3 lower_left_corner;
        vec3 horizontal;
        vec3 vertical;
};

#endif
