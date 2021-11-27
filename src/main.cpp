#include <cmath>
#include <ctime>
#include <iostream>
#include <memory>

// From src/include
#include <camera.hpp>
#include <materials/diffuse.hpp>
#include <objects/sphere.hpp>
#include <stdint.h>
#include <stdlib.h>
#include <utils.hpp>
#include <utils/image.hpp>

// add define to get the error handling macro
#define ERROR_HANDLER_MACRO
#include <common.hpp>

int main(int argc, char * noalias argv[]) {

    const AspectRatio aspect_ratio(3, 2);
    const uint32_t height = 500;
    const uint32_t width = height * aspect_ratio.value();

    image::Image img(width, height);

    Camera cam(0.5 * point3::Z, -point3::Z, vec3::Y, 60, aspect_ratio);

    std::shared_ptr<Diffuse> grey = std::make_shared<Diffuse>(Diffuse(0.5 * colour::WHITE));
    std::shared_ptr<Diffuse> red = std::make_shared<Diffuse>(Diffuse(0.75 * colour::RED));
    std::shared_ptr<Diffuse> purple = std::make_shared<Diffuse>(Diffuse(Colour(0x800080)));

    /* Define scene objects */
    Sphere ball(-point3::Z, 0.5, grey);
    Sphere ball2(Vec3(0.5, 0.1, -1.5), 0.5, red);
    Sphere ground(Vec3(0, -100, 0), 99.5, purple);

    /* Create world and add objects to it */
    HittableList world;
    world.add(ball);
    world.add(ball2);
    world.add(ground);

    constexpr int spp = 100;
    constexpr double scale = 1.0 / double(spp);
    constexpr int max_bounces = 10;

    constexpr int height_by_ten = height / 10;

    for (int j = height; j != 0; j--) {
        for (int i = 0; i != width; i++) {
            Colour c;
            for (int k = 0; k < spp; k++) {
                double u = (double(i) + rng::gen()) / double(width - 1);
                double v = (double(j - 1) + rng::gen()) / double(height - 1);
                Ray r = cam.get_ray(u, v);
                Colour ray_color = 0.75 * colour::WHITE;
                HitRecord h;
                int iter = 0;
                while (iter < max_bounces) {
                    if (world.hit(r, utils::EPSILON, utils::INF, h)) {
                        iter++;
                        h.material->scatter(h, r, ray_color);
                    } else {
                        break;
                    }
                }

                c += ray_color;
            }
            img.push(c * scale);
        }
        if (!(j % height_by_ten))
            std::cout << 100 * (height - j) / height << "%" << std::endl;
    }
    std::cout << "100% complete" << std::endl;

    img.save("image.ppm");

    return EXIT_SUCCESS;
}
