#include "color.h"
#include "ray.h"
#include "vec3.h"

#include <iostream>

// 구에서 t에 대한 이차 방정식
// t^2 * (b dot b) + t * 2 * (b dot (A - C)) + ((A - C) dot (A - C) - r^2) = 0
bool hit_sphere(const point3& center, double radius, const ray& r) {
    vec3 oc = r.origin() - center; // = (A - C)
    auto a = dot(r.direction(), r.direction()); // = a항 = (b dot b)
    auto b = 2.0 * dot(oc, r.direction()); // b항 = 2 * (b dot (A - C))
    auto c = dot(oc, oc) - radius*radius; // c항 = ((A - C) dot (A - C)) - r^2
    auto discriminant = b*b - 4*a*c; // a, b, c의 값을 알았으므로 판별식을 통해 구에 ray가 닿았는지 판단.
    return (discriminant > 0); // ray가 구를 뚫었을 때 (판별식 > 0 = t가 2개의 값을 가질 때)
}

color ray_color(const ray& r) {
    // If hit shpere
    if (hit_sphere(point3(0,0,-1), 0.5, r)) // center, r, ray
        return color(0, 1, 0); // set red color
    
    // no hit shpere
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5*(unit_direction.y() + 1.0);
    return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
}

int main() {

    // Image
    const auto aspect_ratio = 16.0 / 9.0; // 1.7777....
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio); // 225

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
            color pixel_color = ray_color(r); // ray가 닿은 곳의 색상 계산
            write_color(std::cout, pixel_color); // 색상 적용.
        }
    }

    std::cerr << "\nDone.\n";

    return 0;
}
