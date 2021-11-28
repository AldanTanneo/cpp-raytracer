// C libs
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <ctime>
// C++ headers
#include <chrono>
#include <iostream>
#include <memory>
// Parallelization lib
#include <omp.h>

// From src/include
#include <camera.hpp>
#include <main.hpp>
#include <materials/diffuse.hpp>
#include <objects/sphere.hpp>
#include <utils/image.hpp>

constexpr AspectRatio aspect_ratio(16, 9);
constexpr uint32_t height = 1080;
constexpr uint32_t width = height * aspect_ratio.value();
constexpr double height_scale = 1.0 / double(height - 1);
constexpr double width_scale = 1.0 / double(width - 1);

constexpr int spp = 100;
constexpr double colour_scale = 1.0 / double(spp);
constexpr int max_bounces = 10;

int main(int argc, char * noalias argv[]) {
    rng::seed(time(0));

    image::Image img = image::Image::black(width, height);

    const Camera cam(0.5 * point3::Z, -point3::Z, vec3::Y, 60, aspect_ratio);

    std::shared_ptr<Diffuse> grey =
        std::make_shared<Diffuse>(Diffuse(0.5 * colour::WHITE));
    std::shared_ptr<Diffuse> red =
        std::make_shared<Diffuse>(Diffuse(0.75 * colour::RED));
    std::shared_ptr<Diffuse> purple =
        std::make_shared<Diffuse>(Diffuse(0.5 * colour::MAGENTA));

    /* Define scene objects */
    const Sphere ball(-point3::Z, 0.5, grey);
    const Sphere ball2(Vec3(0.5, 0.1, -1.5), 0.5, red);
    const Sphere ground(Vec3(0, -100, 0), 99.5, purple);

    /* Create world and add objects to it */
    HittableList world;
    world.add(ball);
    world.add(ball2);
    world.add(ground);

    utils::ProgressBar pb(width * height, 20);
    std::cout << term_colours::BOLD << "Rendering image..." << std::endl;
    pb.start(term_colours::CYAN);

    /* Measure calculations */
    auto clock = std::chrono::system_clock();
    auto time0 = clock.now();

#pragma omp parallel for
    for (size_t index = 0; index != width * height; index++) {
        Colour c;
        uint32_t i = index % width;
        uint32_t j = height - 1 - index / width;
        double u, v;
        for (int k = 0; k < spp; k++) {
            u = (double(i) + rng::gen()) * width_scale;
            v = (double(j) + rng::gen()) * height_scale;
            c += cam.cast_ray<max_bounces>(world, 0.75 * colour::WHITE, u, v);
        }
        img[(height - 1 - j) * width + i] = c * colour_scale;
        pb.advance();
    }

    pb.stop();
    auto duration = clock.now() - time0;

    auto millis =
        std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

    std::cout << "Image rendered in " << millis / 1000 << "s " << millis % 1000
              << "ms" << std::endl;

    img.save("image.ppm");

    return EXIT_SUCCESS;
}
