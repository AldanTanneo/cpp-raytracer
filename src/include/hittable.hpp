#ifndef HITTABLE_HPP
#define HITTABLE_HPP

#include <iterator>
#include <memory>
#include <type_traits>
#include <vector>

// From src/include
// #include <material.hpp>
#include <ray.hpp>
#include <utils/vec3.hpp>

class Material; // Forward declaration of material

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
    std::shared_ptr<Material> material;

    /* Construct an empty hit record */
    inline HitRecord() noexcept
        : time(0.0),
          hit_point(point3::ZEROS),
          surface_normal(point3::ZEROS),
          front_face(false),
          material(std::shared_ptr<Material>()) {}

    /* Set the correct orientation of the normal */
    constexpr void set_face_normal(const Ray & r, const Vec3 & outward_normal) noexcept {
        front_face = r.direction.dot(outward_normal) < 0;
        surface_normal = front_face ? outward_normal : -outward_normal;
    }
};

/* Abstract interface of a material */
class Material {
  public:
    /* Define how a ray should interact with the material */
    virtual bool scatter(const HitRecord & hit_record, Ray & scattered, Color & attenuation) const noexcept = 0;

    /* Virtual destructor */
    virtual ~Material() noexcept = default;
};

/* Abstract interface of a hittable object */
class Hittable {
  public:
    /* Check if a ray hits the hittable object. If there is a hit, return a hit record */
    virtual bool hit(const Ray & ray_in, double tmin, double tmax, HitRecord & hit_record) const noexcept = 0;

    /* Virtual destructor */
    virtual ~Hittable() noexcept = default;
};

/* A dynamic list of hittable objects */
class HittableList : public Hittable {
  public:
    using value_type = std::unique_ptr<Hittable>;
    using container = std::vector<value_type>;
    using const_iterator = container::const_iterator;

  private:
    /* The inner container */
    container objects;

  public:
    /* Construct an empty hittable list */
    inline HittableList() noexcept : objects(container()) {}

    /* Add an object inheriting from the Hittable class */
    template <class T>
    inline void add(T object) noexcept {
        static_assert(std::is_convertible<T *, Hittable *>(), "T must publicly inherit from the Hittable abstract interface!");
        objects.push_back(std::make_unique<T>(object));
    }

    /* Indexing operator overloading */
    constexpr const value_type & operator[](int index) const {
        return objects[index];
    }

    /* const iterator begin definition */
    inline const_iterator begin() const noexcept {
        return objects.begin();
    }

    /* Const iterator end definition */
    inline const_iterator end() const noexcept {
        return objects.end();
    }

    /* Hit method override */
    virtual bool hit(const Ray & ray_in, double tmin, double tmax, HitRecord & hit_record) const noexcept override;
};

#endif
