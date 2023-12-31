#include "rtweekend.h"

#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"

#include <iostream>

color ray_color(const ray& r, const hittable_list& world, const int depth) {
    hit_record rec; // ray hit시 정보를 담는 구조체 rec

	// 재귀가 50번 수행된 경우 (ray가 50번이나 충돌한 경우)
	if (depth <= 0)
		return color(0, 0, 0); // 검정색 출력

    // ray가 충돌한 객체가 있는 경우
    if (world.hit(r, 0.001, infinity, rec)) {
        ray scattered;
        color attenuation;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
		    return attenuation * ray_color(scattered, world, depth - 1);	// P에서 S로 쏜 ray로 ray_color() 재귀 호출
        return color(0, 0, 0);
    }

    // ray가 충돌한 객체가 없는 경우
    vec3 unit_direction = unit_vector(r.direction());	// ray.dir의 단위 벡터로 변환
    auto t = 0.5*(unit_direction.y() + 1.0);	// 0 <= t <= 1
    return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);	// lerp (특정 색으로 blend)
}

int main() {

    // Image
    const auto aspect_ratio = 16.0 / 9.0; // 1.7777....
    const int image_width = 800;
    const int image_height = static_cast<int>(image_width / aspect_ratio); // 225
    const int samples_per_pixel = 50;
	const int max_depth = 50;

    // World
    hittable_list world;    // 객체 리스트를 가지는 world 객체 생성
    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<lambertian>(color(0.7, 0.3, 0.3));
    auto material_left   = make_shared<metal>(color(0.8, 0.8, 0.8));
    auto material_right  = make_shared<metal>(color(0.8, 0.6, 0.2));

    world.add(make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<sphere>(point3( 0.0,    0.0, -1.0),   0.5, material_center));
    world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.5, material_left));
    world.add(make_shared<sphere>(point3( 1.0,    0.0, -1.0),   0.5, material_right));

    // Camera
    camera cam(point3(-2,2,1), point3(0,0,-1), vec3(0,1,0), 90.0, aspect_ratio);

    // Render
    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    // 왼쪽 위부터 렌더링
    for (int j = image_height-1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            color pixel_color(0, 0, 0);	// 픽셀의 초기 RGB 값
			for (int s = 0; s < samples_per_pixel; s++)	// sample 개수 만큼 ray 생성
			{
				auto u = (i + random_double()) / (image_width-1);	// 한 픽셀 내부의 width에 대한 offset
                auto v = (j + random_double()) / (image_height-1);	// 한 픽셀 내부의 height에 대한 offset
				ray r = cam.get_ray(u, v);	// ray 계산
				pixel_color += ray_color(r, world, max_depth); // 한 픽셀 내부에 샘플 수 만큼 ray를 쏘고, 해당 ray들의 색상 합계를 구함
			}
			write_color(std::cout, pixel_color, samples_per_pixel); // 색상 적용
        }
    }

    std::cerr << "\nDone.\n";

    return 0;
}
