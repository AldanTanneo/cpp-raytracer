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
#include <materials/dielectric.hpp>
#include <materials/diffuse.hpp>
#include <materials/emissive.hpp>
#include <materials/metal.hpp>
#include <materials/plastic.hpp>
#include <objects/sphere.hpp>
#include <utils/image.hpp>

constexpr AspectRatio aspect_ratio(16, 9);
constexpr size_t height = 1080;
constexpr size_t width = height * aspect_ratio.value();
constexpr double height_scale = 1.0 / double(height - 1);
constexpr double width_scale = 1.0 / double(width - 1);

constexpr int spp = 200;
constexpr double colour_scale = 1.0 / double(spp);
constexpr int max_bounces = 25;

/* In-processing kernel parameters */
constexpr double processing_kernel_weight = 2;
constexpr double processing_kernel_offset =
    (utils::const_sqrt(1 + processing_kernel_weight) - 1) / 2;
constexpr double processing_kernel_min = 0.0 - processing_kernel_offset;
constexpr double processing_kernel_max = 1.0 + processing_kernel_offset;

const Camera
    cam(Vec3(-0.3, 1, 2), Vec3(-0.25, 0.5, -1.5), vec3::Y, 30, aspect_ratio);

/* Define scene materials */
const Plastic blue = Plastic(Colour(0x0015D7), 1.5);
const BlackBody light = BlackBody(colour::CYAN + 0.05 * colour::MAGENTA, 3.0);
const Plastic ground_material = Plastic(colour::WHITE);

/* Define scene objects */
const Sphere ball(Vec3(0, 0.5, -1.0), 0.25, light);
const Sphere ball2(Vec3(-0.5, 0.5, -2.0), 0.5, blue);
const Sphere ground(Vec3(0, -100, 0), 100, ground_material);

int main(int argc, char * argv[]) {
    DEBUG(processing_kernel_offset);
    /* Initialize the RNG */
    rng::seed(time(0));

    /* Create world and add objects to it */
    HittableList world;
    world.add(ball);
    world.add(ball2);
    world.add(ground);

    /* Create a black image to fill with pixels */
    image::Image img = image::Image::black(width, height);

    utils::ProgressBar pb(width * height);
    std::cout << term_colours::ITALIC << "Rendering image..." << std::endl;

    pb.start(term_colours::CYAN);

#pragma omp parallel for default(none) shared(cam, img, pb, world)             \
    schedule(dynamic)
    for (size_t index = 0; index < width * height; ++index) {
        Colour c;
        const size_t i = index % width;
        const size_t j = height - 1 - (index / width);
        double u, v;
        for (int k = 0; k < spp; ++k) {
            u = (static_cast<double>(i)
                 + rng::gen(processing_kernel_min, processing_kernel_max))
                * width_scale;
            v = (static_cast<double>(j)
                 + rng::gen(processing_kernel_min, processing_kernel_max))
                * height_scale;
            c += cam.cast_ray(world, 0.1 * colour::WHITE, max_bounces, u, v);
        }
        img[index] += c * colour_scale;
        pb.advance();
    }

    pb.stop("Image rendered");

    std::cout << "Applying blur filter..." << std::endl;

    img.gaussian_blur();

    std::cout << "Saving image..." << std::endl;

    img.save("image.ppm");

    return EXIT_SUCCESS;
}
