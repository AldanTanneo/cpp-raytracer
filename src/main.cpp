#include <iostream>
#include <cmath>

// From src/include
#include <image.hpp>
#include <main.hpp>
#include <vec3.hpp>

void test_3d_vectors() {
    Vec3 u(0, 1, 0);
    Vec3 v(sqrt(2) / 2, 0, sqrt(2) / 2);
    std::cout << "u . v = " << u.dot(v) << std::endl;
    std::cout << "u x v = " << u.cross(v) << std::endl;
    std::cout << 3 * Vec3::ONES() << std::endl;
}

int main() {
    test_3d_vectors();

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

    std::cout << "Image1 size: " << img.get_width() << "x" << img.get_height() << std::endl;
    std::cout << "Image2 size: " << img2.get_width() << "x" << img2.get_height() << std::endl;

    std::cout << "Image1 == Image2: " << (img == img2 ? "true" : "false") << std::endl;

    img2.save("image2.ppm");

    return EXIT_SUCCESS;
}
