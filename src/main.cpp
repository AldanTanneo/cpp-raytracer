#include <cmath>
#include <iostream>
#include <memory>

// From src/include
#include <camera.hpp>
#include <main.hpp>
#include <objects/sphere.hpp>
#include <utils.hpp>
#include <utils/rng.hpp>

void test_3d_vectors() {
    Vec3 u(0, 1, 0);
    Vec3 v(sqrt(2) / 2, 0, sqrt(2) / 2);
    std::cout << "u . v = " << u.dot(v) << std::endl;
    std::cout << "u x v = " << u.cross(v) << std::endl;
    std::cout << 3 * vec3::ONES << std::endl;
}

int main(int argc, char * argv[]) {

    int height = 720;
    int width = (height * 16) / 9;

    image::Image img(width, height);

    Camera cam(point3::ZEROS, -point3::Z, vec3::Y, 90, AspectRatio(16, 9));

    Sphere ball(-point3::Z, 0.5);
    Sphere ball2(Vec3(0.2, 0.2, -1.2), 0.5);
    Sphere ground(Vec3(0, -100, 0), 99);
    HittableList world;
    world.add(ball);
    world.add(ball2);
    world.add(ground);
    HitRecord h;

    for (int j = height - 1; j >= 0; j--) {
        for (int i = 0; i < width; i++) {
            double u = double(i) / double(width - 1);
            double v = double(j) / double(height - 1);
            Ray r = cam.get_ray(u, v);

            if (world.hit(r, utils::EPSILON, utils::INF, h)) {
                img.push(0.5 * Colour(h.surface_normal + 1));
            } else {
                img.push(0.8 * colour::WHITE + 0.2 * colour::CYAN);
            }
        }
    }

    img.save("image.ppm");

    return EXIT_SUCCESS;
}
