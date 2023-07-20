#include "color.h"
#include "ray.h"
#include "vec3.h"

#include <iostream>

color ray_color(const ray& r) {
    vec3 unit_direction = unit_vector(r.direction()); // ray의 단위 벡터
    auto t = 0.5*(unit_direction.y() + 1.0); // 0 <= t <= 1.0 으로 변환.
    return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0); // lerp 공식. (두 색상 linear blend)
}

int main() {

    // Image
    const auto aspect_ratio = 16.0 / 9.0; // 1.7777....
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio); // 225

    // Camera
    auto viewport_height = 90.0;
    auto viewport_width = aspect_ratio * viewport_height; // 160
    auto focal_length = 1.0;

    auto origin = point3(0, 0, 0); // ray origin (camera)
    auto horizontal = vec3(viewport_width, 0, 0); // (160, 0, 0)
    auto vertical = vec3(0, viewport_height, 0); // (0, 90, 0)
    auto lower_left_corner = origin - horizontal/2 - vertical/2 - vec3(0, 0, focal_length); // (-80, -45, -1)

    // Render

    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    // 왼쪽 위부터 렌더링
    for (int j = image_height-1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            auto u = double(i) / (image_width-1); // 0.0 -> 1.0
            auto v = double(j) / (image_height-1); // 1.0 -> 0.0
            ray r(origin, lower_left_corner + u*horizontal + v*vertical - origin); // u, v = offset
            std::cout << r.dir << std::endl;
            color pixel_color = ray_color(r);
            //std::cout << pixel_color << std::endl;
            //write_color(std::cout, pixel_color);
        }
    }

    std::cerr << "\nDone.\n";

    return 0;
}
