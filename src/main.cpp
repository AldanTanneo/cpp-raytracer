#include <fstream>
#include <iostream>
#include <vector>

// From src/include
#include <image.hpp>
#include <vec3.hpp>

void test_3d_vectors() {
    vec3 u(0, 1, 0);
    vec3 v(sqrt(2) / 2, 0, sqrt(2) / 2);
    std::cout << "u . v = " << u.dot(v) << std::endl;
    std::cout << "u x v = " << u.cross(v) << std::endl;
}

int main() {
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

    return EXIT_SUCCESS;
}
