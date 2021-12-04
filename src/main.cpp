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
#include <objects/cylinder.hpp>
#include <objects/sphere.hpp>
#include <objects/triangle.hpp>
#include <utils/image.hpp>
#include <utils/progress_bar.hpp>

using namespace std;
using namespace colour;

constexpr AspectRatio aspect_ratio(16, 9);
constexpr size_t height = 1080;
constexpr size_t width = height * aspect_ratio.value();
constexpr double height_scale = 1.0 / double(height - 1);
constexpr double width_scale = 1.0 / double(width - 1);

constexpr int spp = 100;
constexpr double colour_scale = 1.0 / double(spp);
constexpr int max_bounces = 15;

// In-processing kernel parameters
constexpr double processing_kernel_weight = 1;
constexpr double processing_kernel_offset =
    (utils::const_sqrt(1 + processing_kernel_weight) - 1) / 2;
constexpr double processing_kernel_min = 0.0 - processing_kernel_offset;
constexpr double processing_kernel_max = 1.0 + processing_kernel_offset;

// Camera
const Camera
    cam(Point3(3, 3.4, 10.2), Point3(0.5, 0.4, 0), vec3::Y, 15, aspect_ratio);

// Global lights
const vector<GlobalIllumination> global_light = {
    GlobalIllumination(MAGENTA, LightType::Infinite, Vec3(-1, 0.2, 1)),
    GlobalIllumination(20 * BLUE, LightType::Point, Point3(1, 3, -1)),
    GlobalIllumination(0.01 * WHITE)};

// Define scene materials
const Dielectric glass = Dielectric(Colour(0.9 * WHITE), 1.2);
const Diffuse ground_material = Diffuse(0.3 * WHITE);
const Plastic pen_material = Plastic(0.7 * WHITE);

// Define scene objects
const Sphere ground(Point3(0, -2000, 0), 2000, ground_material);
const Cylinder cyl(Point3(0, 0, 0), vec3::Y, 0.5, 0.65, glass);
const Cylinder pen(Point3(0.75, 1, 0), Vec3(-1, -1, 0), 0.05, 3, pen_material);
const Cylinder test(Point3(-0.7, 0, 0.7), vec3::Y, 0.2, 1, pen_material);
const Triangle triangle_1(Point3(1.5, 0.5, 0.5),
                          Point3(2, 0, 0),
                          Point3(1, 0, 0),
                          pen_material);
const Triangle triangle_2(Point3(1.5, 0.5, 0.5),
                          Point3(1, 0, 1),
                          Point3(2, 0, 1),
                          pen_material);
const Triangle triangle_3(Point3(1.5, 0.5, 0.5),
                          Point3(2, 0, 1),
                          Point3(2, 0, 0),
                          pen_material);
const Triangle triangle_4(Point3(1.5, 0.5, 0.5),
                          Point3(1, 0, 0),
                          Point3(1, 0, 1),
                          pen_material);

int main(int argc, char * argv[]) {
    // Initialize the RNG
    rng::seed(time(0));

    // Create world and add objects to it
    HittableList world;
    world.add(ground);
    world.add(cyl);
    world.add(pen);
    world.add(test);

    world.add(triangle_1);
    world.add(triangle_2);
    world.add(triangle_3);
    world.add(triangle_4);

    // Create a black image to fill with pixels
    Image img = Image::black(width, height);

    ProgressBar pb(width * height);
    cout << "Rendering image..." << endl;

    pb.start(term_colours::CYAN);

#pragma omp parallel for default(none)                                         \
    shared(cam, img, pb, world, global_light) schedule(dynamic)
    for (size_t index = 0; index < width * height; ++index) {
        Colour c;
        const size_t i = index % width;
        const size_t j = height - 1 - (index / width);
        for (int k = 0; k < spp; ++k) {
            double u =
                (static_cast<double>(i)
                 + rng::gen(processing_kernel_min, processing_kernel_max))
                * width_scale;
            double v =
                (static_cast<double>(j)
                 + rng::gen(processing_kernel_min, processing_kernel_max))
                * height_scale;
            c += cam.cast_ray(world, global_light, max_bounces, u, v);
        }
        img[index] += c * colour_scale;
        pb.advance();
    }

    pb.stop("Image rendered");

    cout << "Applying blur filter..." << endl;

    img.gaussian_blur();

    cout << "Saving image...";

    img.save("image.ppm");

    cout << " Done!" << endl;

    return EXIT_SUCCESS;
}
