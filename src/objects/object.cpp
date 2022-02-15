#include <cmath>

// From src/include
#include <hittable.hpp>
#include <objects/object.hpp>
#include <utils/vec3.hpp>

std::vector<Triangle>
    Object::triangles_from_file(const std::string & obj_file_name) noexcept {
    std::ifstream obj_file(obj_file_name);
    if (!obj_file.is_open()) {
        std::cout << "/!\\ Unable to open " << obj_file_name << " /!\\"
                  << std::endl;
    }

    std::vector<Triangle> triangles_set;
    // vector corresponding to the points in the .obj file makema
    std::vector<Point3> obj_points;
    // current word in the file
    std::string word;
    // positions of the vertices of the triangles
    int vertices_index[3];
    // used to split the words to get the vertex index in the obj (see end)
    int slash_position;

    while (obj_file >> word) {
        // Creates 3d points and corresponding triangles from .obj file
        if (word == "v") {
            Point3 point;
            obj_file >> word;
            point.x = stod(word);
            obj_file >> word;
            point.y = stod(word);
            obj_file >> word;
            point.z = stod(word);
            obj_points.push_back(point);
        }

        // assuming the polygon is a triangle, creates and adds the triangle to
        // the list
        if (word == "f") {
            for (size_t i = 0; i < 3; ++i) {
                obj_file >> word;
                slash_position = word.find('/');
                vertices_index[i] = stoi(word.substr(0, slash_position));
            }
            // creates the triangle from the given points and pushes it
            Triangle triangle(obj_points.data()[vertices_index[0]],
                              obj_points.data()[vertices_index[1]],
                              obj_points.data()[vertices_index[2]], material);
            triangles_set.push_back(triangle);
        }
    }

    obj_file.close();
    return triangles_set;
}

bool Object::hit(const Ray & ray,
                 double tmin,
                 double tmax,
                 HitRecord & hit_record) const noexcept {
    // Checks every triangle
    for (const Triangle & triangle : triangles_set) {
        triangle.hit(ray, tmin, tmax, hit_record);
    }

    return false;
}
