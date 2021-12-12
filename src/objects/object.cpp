#include <cmath>

// From src/include
#include <hittable.hpp>
#include <objects/object.hpp>
#include <utils/vec3.hpp>

/* gets the position of the vertices of the triangle from the f format */
int vertices_index_from_f(std::string f_format)
{
  int compt = 0;
  int res = 0;
  while (f_format.at(compt) != '/') {
    compt++;
  }

  for (size_t i = 0; i < compt; i++) {
    res += ((int)f_format.at(compt - 1 - i) - 48) * pow(10, i);
  }
  return res;
}



std::vector<Triangle> Object::triangles_from_file(const std::string & obj_file_name) noexcept { 

    std::ifstream obj_file(obj_file_name);
    if (!obj_file.is_open()) {
        std::cout << "/!\\ Unable to open " << obj_file_name << " /!\\" << std::endl;
    }
  
    std::vector<Triangle> triangles_set;
    // vector corresponding to the points in the .obj file 
    std::vector<Point3> obj_points;
    // values of the coordinates
    double position;
    std::string word;
    // contains the positions of the vertices of the triangles
    int i1, i2, i3;
    // Creates 3d points and corresponding triangles from .obj file
    while (obj_file >> word)
    {
        if (word == "v") {
            Point3 point;
            obj_file >> position;
            point.x = position;
            obj_file >> position;
            point.y = position;
            obj_file >> position;
            point.z = position;
            obj_points.push_back(point);
        }

        if (word == "f") {
            // assuming the polygon is a triangle, 
            // creates and adds the triangle to the list
            obj_file >> word;
            i1 = vertices_index_from_f(word) - 1;
            obj_file >> word;
            i2 = vertices_index_from_f(word) - 1;
            obj_file >> word;
            i3 = vertices_index_from_f(word) - 1;
            // creates the triangle from the given points and pushes it
            Triangle triangle(obj_points.data()[i1], obj_points.data()[i2], obj_points.data()[i3], material);
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
    for (auto triangle = triangles_set.begin(); triangle != triangles_set.end(); ++triangle)
    {
        triangle->hit(ray, tmin, tmax, hit_record);
    }
    
    return false;
}