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
#include <string>
// Parallelization lib
#include <omp.h>

// From src/include
#include <camera.hpp>
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

int main(int argc, char * argv[]) try {
    // Initialize the RNG for all threads
#pragma omp parallel
    {
        rng::seed(time(0));
        rng::jump(omp_get_thread_num());
    }

    if (argc < 2) {
        cout << "Error: missing config file." << endl
             << endl
             << "Usage: xtrem-raytracer.exe <json config file>" << endl;
        return -1;
    }

    Params params;
    try {
        params = Params::load_params(argv[1]);
    } catch (const std::exception & e) {
        cout << "Error parsing config file: " << e.what() << endl;
        return -1;
    } catch (const std::string & s) {
        cout << "Error parsing config file: " << s << endl;
    } catch (const char * s) {
        cout << "Error parsing config file: " << s << endl;
        return -1;
    }

    const size_t height = params.info.height;
    const size_t width = params.info.height * params.info.aspect_ratio.value();

    const double height_scale = 1.0 / double(height - 1);
    const double width_scale = 1.0 / double(width - 1);

    const int spp = params.info.spp;
    DEBUG(spp);
    const double spp_scale = 1.0 / double(spp);
    const double half_spp_scale = 2.0 / double(spp);
    const int max_bounces = params.info.max_bounces;
    DEBUG(max_bounces);

    const Camera cam = params.cam;

    const vector<GlobalIllumination> global_lights = params.global_lights;

    HittableList world;
    for (const std::shared_ptr<Hittable> & obj : params.objects) {
        world.add((const Hittable &)*obj);
    }

    HittableList sampled_hittables;
    for (const size_t & index : params.sampled_objects) {
        const Hittable & obj = *params.objects[index];
        sampled_hittables.add(obj);
    }

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
        img[index] = (pixel_colour[0] + pixel_colour[1]) * spp_scale;

        // fill half buffers with variance
        double l0 = pixel_colour[0].luminance() * half_spp_scale;
        var[0][index] = var[0][index] * half_spp_scale - l0 * l0;
        double l1 = pixel_colour[1].luminance() * half_spp_scale;
        var[1][index] = var[1][index] * half_spp_scale - l1 * l1;

        pb.advance();
    }

    pb.stop("Image rendered");

    cout << "Applying firefly filter..." << endl;

    // img.clamp();
    img.fireflies_filter(var[0], var[1]);

    cout << "Saving image...";

    img.save_png("image.png");
    Image::from_grayscale(var[0], width, height).save_png("variance.png");

    cout << " Done!" << endl;

    return EXIT_SUCCESS;

} catch (const char * s) {
    std::cout << "Uncaught exception: " << s << std::endl;
    return -1;
} catch (const std::string & s) {
    std::cout << "Uncaught exception: " << s << std::endl;
    return -1;
} catch (const std::exception & e) {
    std::cout << "Uncaught exception: " << e.what() << std::endl;
    return -1;
} catch (...) {
    std::cout << "Uncaught exception: unknown exception" << std::endl;
    return -1;
}
