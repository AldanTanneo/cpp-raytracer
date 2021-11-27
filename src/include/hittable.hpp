#ifndef HITTABLE_HPP
#define HITTABLE_HPP

#include <iterator>
#include <memory>
#include <vector>

// From src/include
// #include <material.hpp>
#include <ray.hpp>
#include <utils/vec3.hpp>

class Material;

/* The returned structure if a ray hits an object */
struct HitRecord {
    double time;
    /* The point of intersection between the ray and the object */
    Point3 hit_point;
    /* The normal of the object surface at the intersection point */
    Vec3 surface_normal;
    /* Determines if the surface is hit from the front or the back */
    bool front_face;
    /* The material of the object surface */
    // std::shared_ptr<Material> mat;

    /* Construct an empty hit record */
    inline HitRecord() noexcept
        : time(0.0),
          hit_point(point3::ZEROS),
          surface_normal(point3::ZEROS),
          front_face(false) {}

    /* Set the correct orientation of the normal */
    constexpr void set_face_normal(const Ray & r, const Vec3 & outward_normal) noexcept {
        front_face = r.direction.dot(outward_normal) < 0;
        surface_normal = front_face ? outward_normal : -outward_normal;
    }
};

class Material {
  public:
    /* Define how a ray should interact with the material */
    virtual bool scatter(const Ray & ray_in, const HitRecord & hit_record, Ray & scattered, Color & attenuation) const noexcept = 0;

    /* Get the material's colour */
    virtual Colour colour() const noexcept = 0;

    /* Virtual destructor */
    virtual ~Material() noexcept = default;
};

/* Abstract class of a hittable object */
class Hittable {
  public:
    /* Check if a ray hits the hittable object. If there is a hit, return a hit record */
    virtual bool hit(const Ray & ray_in, double tmin, double tmax, HitRecord & hit_record) const noexcept = 0;

    /* Virtual destructor */
    virtual ~Hittable() noexcept = default;
};

/* A dynamic list of hittable objects */
class HittableList : public Hittable {
  private:
    /* The inner container */
    std::vector<std::unique_ptr<Hittable>> objects;

  public:
    /* Construct an empty hittable list */
    inline HittableList() noexcept : objects(std::vector<std::unique_ptr<Hittable>>()) {}

    /* Add an object inheriting from the Hittable class */
    template <class T>
    void add(T object) noexcept {
        objects.push_back(std::make_unique<T>(object));
    }

    /* Indexing operator overloading */
    constexpr const std::unique_ptr<Hittable> & operator[](int index) const {
        return objects[index];
    }

    /* const iterator begin definition */
    inline std::vector<std::unique_ptr<Hittable>>::const_iterator begin() const noexcept {
        return objects.begin();
    }

    /* Const iterator end definition */
    inline std::vector<std::unique_ptr<Hittable>>::const_iterator end() const noexcept {
        return objects.end();
    }

    /* Hit method override */
    virtual bool hit(const Ray & ray_in, double tmin, double tmax, HitRecord & hit_record) const noexcept override;
};

#endif
