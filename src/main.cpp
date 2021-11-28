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
constexpr size_t height = 1080;
constexpr size_t width = height * aspect_ratio.value();
constexpr double height_scale = 1.0 / double(height - 1);
constexpr double width_scale = 1.0 / double(width - 1);

constexpr int spp = 100;
constexpr double colour_scale = 1.0 / double(spp);
constexpr int max_bounces = 10;

int main(int argc, char * noalias argv[]) {
    /* Initialize the RNG */
    rng::seed(time(0));

    const Camera cam(0.5 * point3::Z, -point3::Z, vec3::Y, 60, aspect_ratio);

    /* Define scene materials */
    const Diffuse grey = Diffuse(0.5 * colour::WHITE);
    const Diffuse red = Diffuse(0.75 * colour::RED);
    const Diffuse purple = Diffuse(0.5 * colour::MAGENTA);

    Sphere ball(-point3::Z, 0.5, grey);
    Sphere ball2(Vec3(0.5, 0.1, -1.5), 0.5, red);
    Sphere ground(Vec3(0, -100, 0), 99.5, purple);

    /* Create world and add objects to it */
    HittableList world;
    world.add(ball);
    world.add(ball2);
    world.add(ground);

    /* Create a black image to fill with pixels */
    image::Image img = image::Image::black(width, height);

    utils::ProgressBar pb(width * height);
    std::cout << term_colours::BOLD << "Rendering image..." << std::endl;

    pb.start(term_colours::CYAN);

#pragma omp parallel for
    for (size_t index = 0; index != width * height; index++) {
        Colour c;
        const size_t i = index % width;
        const size_t j = height - 1 - (index / width);
        double u, v;
        for (int k = 0; k < spp; k++) {
            u = (static_cast<double>(i) + rng::gen()) * width_scale;
            v = (static_cast<double>(j) + rng::gen()) * height_scale;
            c += cam.cast_ray(world, 0.75 * colour::WHITE, max_bounces, u, v);
        }
        img[index] = c * colour_scale;
        pb.advance();
    }

    pb.stop("Image rendered");

    img.save("image.ppm");

    return EXIT_SUCCESS;
}
