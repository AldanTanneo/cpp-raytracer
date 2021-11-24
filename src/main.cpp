#include <cmath>
#include <iostream>

// From src/include
#include <camera.hpp>
#include <image.hpp>
#include <main.hpp>
#include <vec3.hpp>

void test_camera() {
    Camera cam(Point3::ZEROS(), Point3::X(), Vec3::Y(), 90, AspectRatio(1));

    DBG(cam.get_ray(0.5, 0.5)); // looking right in the middle of the screen: direction = X
    DBG(cam.get_ray(0, 0));     // bottom left corner of the screen
    DBG(cam.get_ray(1, 1));     // top right corner of the screen
    std::cout << std::endl;
}

int main() {
    test_camera();

    int width = 200;
    int height = 200;

    Image img(width, height);

    for (int j = height - 1; j >= 0; j--) {
        for (int i = 0; i < width; i++) {
            float r = float(i) / float(width - 1);
            float g = float(j) / float(height - 1);
            float b = (1.0 - r) * (1.0 - g);

            img.push(r, g, b);
        }
    }

    img.save("image.ppm");

    Image img2 = Image::load("image.ppm");

    DBG(img.get_width());
    DBG(img.get_height());

    std::cout << std::endl;

    DBG(img2.get_width());
    DBG(img2.get_height());

    std::cout << std::endl;

    DBG(img == img2);

    img2.save("image2.ppm");

    return EXIT_SUCCESS;
}
