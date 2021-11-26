#include <cmath>
#include <cstdlib>
#include <iostream>

// From src/include
#include <camera.hpp>
#include <image.hpp>
#include <main.hpp>
#include <vcruntime_new.h>
#include <vec3.hpp>

void tests() {
    Camera cam(vec3::ZEROS, vec3::X, vec3::Y, 90, AspectRatio(1));

    DEBUG(cam.get_ray(0.5, 0.5), "looking right in the middle of the screen: direction = X");
    DEBUG(cam.get_ray(0, 0), "bottom left corner of the screen");
    DEBUG(cam.get_ray(1, 1), "top right corner of the screen");
    newline();

    Vec3 u(1, -1, 1);
    DEBUG(u);
    DEBUG(u.reflect(vec3::Y), "Reflect u along the (x, z) plane, defined by its normal y");
    DEBUG(u + vec3::Y);
    newline();

    Colour test = 0.503 * colour::RED + 0.7 * colour::BLUE + 0.3 * colour::GREEN;
    DEBUG(test);
    DEBUG(test.to_hex());
    std::cout << "0x" << std::hex << test.to_hex() << std::endl;
    DEBUG("test");
    newline();
}

int main(int argc, char * argv[]) {
    tests();

    int width = 200;
    int height = 200;

    image::Image img(width, height);

    for (int j = height - 1; j >= 0; j--) {
        for (int i = 0; i < width; i++) {
            double r = double(i) / double(width - 1);
            double g = double(j) / double(height - 1);
            double b = (1.0 - r) * (1.0 - g);

            img.push(r, g, b);
        }
    }

    img.save("image.ppm");

    image::Image img2 = image::Image::load("image.ppm");

    DEBUG(img.get_width());
    DEBUG(img.get_height());
    newline();

    DEBUG(img2.get_width());
    DEBUG(img2.get_height());
    newline();

    std::cout.setf(std::ios_base::boolalpha);
    DEBUG(img == img2);
    std::cout.unsetf(std::ios_base::boolalpha);

    img2.save("image2.ppm");

    return EXIT_SUCCESS;
}
