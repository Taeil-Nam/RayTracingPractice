#include "rtweekend.h"

#include "color.h"
#include "hittable_list.h"
#include "sphere.h"

#include <iostream>

color ray_color(const ray& r, const hittable_list& world) {
    hit_record rec; // ray hit시 정보를 담는 구조체 rec

    // ray가 충돌한 객체가 있는 경우
    if (world.hit(r, 0, infinity, rec)) {
        return 0.5 * (rec.normal + color(1,1,1));   // 해당 객체의 법선(단위 벡터) 요소에 맞는 색으로 변환
    }

    // ray가 충돌한 객체가 없는 경우
    vec3 unit_direction = unit_vector(r.direction());	// ray.dir의 단위 벡터로 변환
    auto t = 0.5*(unit_direction.y() + 1.0);	// 0 <= t <= 1
    return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);	// lerp (특정 색으로 blend)
}

int main() {

    // Image
    const auto aspect_ratio = 16.0 / 9.0; // 1.7777....
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio); // 225

    // World
    hittable_list world;    // 객체 리스트를 가지는 world 객체 생성
    world.add(make_shared<sphere>(point3(0,0,-1), 0.5));        // 구 객체 1개 추가
    world.add(make_shared<sphere>(point3(0,-100.5,-1), 100));   // 구 객체 1개 추가

    // Camera
    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height; // 3.5
    auto focal_length = 1.0; // 초점거리 (카메라와 viewport의 거리)

    auto origin = point3(0, 0, 0); // ray가 생성되는 곳 (카메라)
    auto horizontal = vec3(viewport_width, 0, 0); // (3.5, 0, 0)
    auto vertical = vec3(0, viewport_height, 0); // (2.0, 0, 0)
    auto lower_left_corner = origin - horizontal/2 - vertical/2 - vec3(0, 0, focal_length); // (-1.75, -1, -1)

    // Render
    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    // 왼쪽 위부터 렌더링
    for (int j = image_height-1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            auto u = double(i) / (image_width-1); // 0.0 -> 1.0
            auto v = double(j) / (image_height-1); // 1.0 -> 0.0
            ray r(origin, lower_left_corner + u*horizontal + v*vertical - origin); // u, v = offset, viewport의 왼쪽 위부터 쏨
            color pixel_color = ray_color(r, world); // ray가 닿은 곳의 색상 계산
            write_color(std::cout, pixel_color); // 색상 적용.
        }
    }

    std::cerr << "\nDone.\n";

    return 0;
}
