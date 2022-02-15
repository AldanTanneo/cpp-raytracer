#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <type_traits>
#include <fstream>

// From src/include
#include <hittable.hpp>
#include <utils/vec3.hpp>
#include <objects/triangle.hpp>

// Object class (based on .obj files)
class Object : public Hittable {
private:
    // material corresponding to object
     const Material & material;
    // the set of triangles created from the polygones of the .obj
    std::vector<Triangle> triangles_set;

public:
    // Construct an object from a given .obj file
    template <class T>
    requires Material::is_material<T>
    inline Object(const std::string & obj_file_name, const T & material) noexcept{}


    // Virtual function override
    virtual bool hit(const Ray & ray_in,
                     double tmin,
                     double tmax,
                     HitRecord & hit_record) const noexcept override;
};

#endif
