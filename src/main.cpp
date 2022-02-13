// C libs
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <ctime>
// C++ headers
#include <algorithm>
#include <chrono>
#include <functional>
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
#include <utils/load_json.hpp>
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
constexpr size_t height = 2000;
constexpr size_t width = height * aspect_ratio.value();
constexpr double height_scale = 1.0 / double(height - 1);
constexpr double width_scale = 1.0 / double(width - 1);
// Render
constexpr int spp = 500;
constexpr double half_spp_scale = 2.0 / double(spp);
constexpr int max_bounces = 20;

// Global lights
const vector<GlobalIllumination> global_lights = {};

// Define scene materials
const MicroFacet mat_left(0.95 * BLUE, 20);
const Metal mat_right(0.9 * RED, 0.0);
const Lambertian mat_ground(0.6 * WHITE);
const Plastic mat_back(0.9 * WHITE, 0.1);
const BlackBody mat_light(WHITE, 15);
const Plastic mat_plastic1(WHITE);
const Dielectric mat_plastic2(WHITE, 1.5);

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
const Sphere sphere1(Point3(370, 150, 370), 150, mat_plastic1);
const Sphere sphere2(Point3(150, 90, 150), 90, mat_plastic2);

// Camera
const Camera cam(Point3(277.5, 277.5, -800),
                 Point3(277.5, 277.5, 0),
                 vec3::Y,
                 38.15,
                 aspect_ratio);

constexpr bool compare_luminance(const Colour & a, const Colour & b) noexcept {
    return a.luminance() < b.luminance();
}

int main(int argc, char * argv[]) {
    // Initialize the RNG
#pragma omp parallel
    {
        rng::seed(time(0));
        rng::jump(omp_get_thread_num());
    }

    // const vector<double> gaussian_coeffs = compute_gaussian_coeffs(spp,
    // 0.05);

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

    HittableList sampled_hittables;
    sampled_hittables.add(light);
    // sampled_hittables.add(sphere1);
    // sampled_hittables.add(sphere2);

    // Create two half buffer images to fill with pixels
    Image img(width, height);

    vector<double> var[2] = { vector<double>(width * height),
                              vector<double>(width * height) };

    ProgressBar pb(width * height);
    cout << "Rendering image..." << endl;

    pb.start(term_colours::CYAN);

#pragma omp parallel for schedule(dynamic)
    for (size_t index = 0; index < width * height; ++index) {
        // Colour c;
        const size_t i = index % width;
        const size_t j = height - 1 - (index / width);

        Colour pixel_colour[2];
        for (int k = 0; k < spp; ++k) {
            // Compute camera coordinates
            double u =
                (static_cast<double>(i)
                 + rng::gen(processing_kernel_min, processing_kernel_max))
                * width_scale;
            double v =
                (static_cast<double>(j)
                 + rng::gen(processing_kernel_min, processing_kernel_max))
                * height_scale;
            // Cast ray into scene
            Colour c = cam.cast_ray(world, global_lights, sampled_hittables,
                                    max_bounces, u, v);
            // Add it to the pixel colour (separated in half buffers)
            pixel_colour[k & 1] += c;
            // Compute luminance and squared luminance
            double luminance = c.luminance();
            var[k & 1][index] += luminance * luminance;
        }
        // Compute final pixel
        img[index] = (pixel_colour[0] + pixel_colour[1]) / spp;

        // fill half buffers with variance
        double l0 = pixel_colour[0].luminance() * half_spp_scale;
        var[0][index] = var[0][index] * half_spp_scale - l0 * l0;
        double l1 = pixel_colour[1].luminance() * half_spp_scale;
        var[1][index] = var[1][index] * half_spp_scale - l1 * l1;

        pb.advance();
    }

    pb.stop("Image rendered");

    cout << "Applying firefly filter..." << endl;

    img.clamp();
    img.fireflies_filter(var[0], var[1]);

    cout << "Saving image...";

    img.save_png("image.png");
    Image::from_grayscale(var[0], width, height).save_png("variance.png");

    cout << " Done!" << endl;

    return EXIT_SUCCESS;
}
