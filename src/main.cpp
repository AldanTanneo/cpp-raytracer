#include <cmath>
#include <iostream>
#include <memory>

// From src/include
#include <camera.hpp>
#include <image.hpp>
#include <main.hpp>
#include <objects/sphere.hpp>
#include <utils.hpp>


void test_3d_vectors() {
    Vec3 u(0, 1, 0);
    Vec3 v(sqrt(2) / 2, 0, sqrt(2) / 2);
    std::cout << "u . v = " << u.dot(v) << std::endl;
    std::cout << "u x v = " << u.cross(v) << std::endl;
    std::cout << 3 * vec3::ONES << std::endl;
}

int main(int argc, char * argv[]) {

    int height = 1080;
    int width = (height * 16) / 9;

    image::Image img(width, height);

    Camera cam(point3::ZEROS, -point3::Z, vec3::Y, 90, AspectRatio(16, 9));

    /* Define scene objects */
    Sphere ball(-point3::Z, 0.5);
    Sphere ball2(Vec3(0.2, 0.2, -1.2), 0.5);
    Sphere ground(Vec3(0, -100, 0), 99);
    /* Create world and add objects to it */
    HittableList world;
    world.add(ball);
    world.add(ball2);
    world.add(ground);

    uint64_t spp = 100;

    for (int64_t j = height - 1; j >= 0; j--) {
        for (int64_t i = 0; i < width; i++) {
            Colour c;
            for (int k = 0; k < spp; k++) {
                double u = (double(i) + rng::gen()) / double(width - 1);
                double v = (double(j) + rng::gen()) / double(height - 1);
                Ray r = cam.get_ray(u, v);
                HitRecord h;

                if (world.hit(r, utils::EPSILON, utils::INF, h)) {
                    c += 0.5 * Colour(h.surface_normal + 1);
                } else {
                    c += 0.8 * colour::WHITE + 0.2 * colour::CYAN;
                }
            }
            img.push(c / double(spp));
        }
        if (!(j % 50))
            DEBUG(j);
    }

    img.save("image.ppm");

    return EXIT_SUCCESS;
}
