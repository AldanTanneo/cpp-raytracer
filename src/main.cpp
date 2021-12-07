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
#include <objects/parallelogram.hpp>
#include <objects/sphere.hpp>
#include <objects/triangle.hpp>
#include <utils/image.hpp>
#include <utils/progress_bar.hpp>

using std::cout;
using std::endl;
using std::vector;
using namespace colour;

// In-processing kernel parameters
constexpr double processing_kernel_weight = 1;
constexpr double processing_kernel_offset =
    (utils::const_sqrt(1 + processing_kernel_weight) - 1) / 2;
constexpr double processing_kernel_min = 0.0 - processing_kernel_offset;
constexpr double processing_kernel_max = 1.0 + processing_kernel_offset;

// Image
constexpr AspectRatio aspect_ratio(1);
constexpr size_t height = 1000;
constexpr size_t width = height * aspect_ratio.value();
constexpr double height_scale = 1.0 / double(height - 1);
constexpr double width_scale = 1.0 / double(width - 1);
// Render
constexpr int spp = 500;
constexpr double colour_scale = 1.0 / double(spp);
constexpr int max_bounces = 15;

// Global lights
const vector<GlobalIllumination> global_lights = {};

// Define scene materials
const Plastic mat_left(1.0 * RED, 1);
const Plastic mat_right(0.8 * BLUE, 1);
const Diffuse mat_ground(0.6 * WHITE);
const Plastic mat_back(WHITE, 0.1);
const BlackBody mat_light(WHITE, 15);
const Dielectric mat_plastic(0.75 * WHITE, 1.5);

// Define scene objects
const Parallelogram
    back(Point3(0, 0, 555), Point3(0, 555, 555), Point3(555, 0, 555), mat_back);
const Parallelogram
    right(Point3(0, 0, 0), Point3(0, 555, 0), Point3(0, 0, 555), mat_right);
const Parallelogram
    left(Point3(555, 0, 0), Point3(555, 555, 0), Point3(555, 0, 555), mat_left);
const Parallelogram
    ground(Point3(0, 0, 0), Point3(555, 0, 0), Point3(0, 0, 555), mat_ground);
const Parallelogram ceiling(Point3(0, 555, 0),
                            Point3(555, 555, 0),
                            Point3(0, 555, 555),
                            mat_ground);
const Parallelogram light(Point3(200, 554, 200),
                          Point3(355, 554, 200),
                          Point3(200, 554, 355),
                          mat_light);
const Sphere sphere1(Point3(400, 100, 400), 100, mat_plastic);
const Sphere sphere2(Point3(150, 90, 150), 90, mat_plastic);

const Hittable & sampled_hittable = light;

// Camera
const Camera cam(Point3(277.5, 277.5, -800),
                 Point3(277.5, 277.5, 0),
                 vec3::Y,
                 38.15,
                 aspect_ratio);

int main(int argc, char * argv[]) {
    // Initialize the RNG
    rng::seed(time(0));

    // Create world and add objects to it
    HittableList world;
    world.add(ground);
    world.add(back);
    world.add(ceiling);
    world.add(left);
    world.add(right);
    world.add(sphere1);
    world.add(sphere2);
    world.add(light);

    // Create a black image to fill with pixels
    Image img = Image::black(width, height);

    ProgressBar pb(width * height);
    cout << "Rendering image..." << endl;

    pb.start(term_colours::CYAN);

#pragma omp parallel for schedule(dynamic)
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
            c += cam.cast_ray(world, global_lights, sampled_hittable,
                              max_bounces, u, v);
        }
        img[index] += c * colour_scale;
        pb.advance();
    }

    pb.stop("Image rendered");

    cout << "Applying blur filter..." << endl;

    img.gaussian_blur();

    cout << "Saving image...";

    img.save_png("image.png");

    cout << " Done!" << endl;

    return EXIT_SUCCESS;
}
