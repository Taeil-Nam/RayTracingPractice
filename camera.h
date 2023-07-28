#ifndef CAMERA_H
#define CAMERA_H

#include "rtweekend.h"

class camera {
    public:
        camera(double vfov, double aspect_ratio)
        {
            auto theta = degrees_to_radians(vfov);  // vfov(시야각)을 radian으로 저장
            auto h = tan(theta / 2);    // height의 절반 길이인 h 계산
            auto viewport_height = 2.0 * h; // height = 2h
    		auto viewport_width = aspect_ratio * viewport_height; // 3.5
    		
            auto focal_length = 1.0; // 초점거리 (카메라와 viewport의 거리)

			origin = point3(0, 0, 0); // ray가 생성되는 곳 (카메라)
			horizontal = vec3(viewport_width, 0, 0); // (3.5, 0, 0)
			vertical = vec3(0, viewport_height, 0); // (2.0, 0, 0)
			lower_left_corner = origin - horizontal/2 - vertical/2 - vec3(0, 0, focal_length); // (-1.75, -1, -1)
        }

        ray get_ray(double u, double v) const {
            return ray(origin, lower_left_corner + u*horizontal + v*vertical - origin);
        }

    private:
        point3 origin;
        point3 lower_left_corner;
        vec3 horizontal;
        vec3 vertical;
};

#endif
