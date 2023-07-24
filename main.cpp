#include "color.h"
#include "ray.h"
#include "vec3.h"

#include <iostream>

/* 구에서 t에 대한 이차 방정식 */
// t^2 * (b dot b) + t * 2 * (b dot (A - C)) + ((A - C) dot (A - C) - r^2) = 0

/* 근의 공식 */
// 1. 일반 b인 경우 : (-b - sqrt(b*b - 4*a*c)) / (2.0 * a)
// 2. half b인 경우 : (-b - sqrt(b*b - a*c)) / a

double hit_sphere(const point3& center, double radius, const ray& r) {
    vec3 oc = r.origin() - center; // = (A - C)
    auto a = r.direction().length_squared(); // = a항 = (b dot b) -> 자기 자신(b)의 내적 = 자신(b)의 길이^2
    auto half_b = dot(oc, r.direction()); // b항 = 2 * (b dot (A - C)) -> half_b의 경우 : (b dot (A - C))
    auto c = oc.length_squared() - radius*radius; // c항 = ((A - C) dot (A - C)) - r^2
    auto discriminant = half_b*half_b - a*c; // a, b, c의 값을 알았으므로 판별식을 통해 구에 ray가 닿았는지 판단.

    // 안맞은 경우
    if (discriminant < 0)
        return (-1.0); // -1 반환
    
    // 맞은 경우
    else
        return (-half_b - sqrt(discriminant)) / a; // 근의 공식으로 작은 값의 t를 구해서 반환.
}

color ray_color(const ray& r) {
    auto t = hit_sphere(point3(0,0,-1), 0.5, r); // center, r, ray
    
    // If hit shpere
    if (t > 0.0)
    {
        // N = 구 표면의 법선 벡터의 단위 벡터.
        vec3 N = unit_vector(r.at(t) - vec3(0,0,-1)); // ray가 맞은 표면의 점 - 구의 중심의 벡터를 단위 벡터로 변환
        return 0.5*color(N.x()+1, N.y()+1, N.z()+1); // 음수 값일 수도 있으므로, 1을 더해주고 0.5를 곱한다.
    }
    
    // no hit shpere
    vec3 unit_direction = unit_vector(r.direction());
    t = 0.5*(unit_direction.y() + 1.0);
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
