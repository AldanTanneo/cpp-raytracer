#ifndef HITTABLE_HPP
#define HITTABLE_HPP

#include <memory>
#include <type_traits>
#include <vector>

// From src/include
#include <ray.hpp>
#include <utils/vec3.hpp>

class HitRecord; // Forward declaration of HitRecord

// Abstract interface of a material
class Material {
public:
    template <class T>
    constexpr static bool is_material = std::is_convertible_v<T *, Material *>;

    enum ScatterType {
        None = 0,
        Bounce = 1,
        Emit = 2,
    };
    // Define how a ray should interact with the material.
    // The "ray" object is modified as well as the "ray_colour" object.
    // If no ray is emitted after hitting the material, this function returns
    // false
    virtual ScatterType scatter(const HitRecord & hit_record,
                                Ray & ray,
                                Colour & ray_colour) const noexcept = 0;

    // Virtual destructor
    virtual ~Material() noexcept = default;
};

// Dummy material class for HitRecord empty initialization
class _DummyMaterial : public Material {
public:
    // Static DummyMaterial instance
    static const _DummyMaterial d;

private:
    // Construct a const DummyMaterial instance
    constexpr _DummyMaterial() noexcept {}

    // Never scatter
    virtual ScatterType
    scatter(const HitRecord &, Ray &, Colour &) const noexcept override {
        return None;
    }
};

// The returned structure if a ray hits an object
class HitRecord {
public:
    double time;
    // The point of intersection between the ray and the object
    Point3 hit_point;
    // The normal of the object surface at the intersection point.
    // Must be a unit vector.
    Vec3 surface_normal;
    // Determines if the surface is hit from the front or the back
    bool front_face;
    // The material of the object surface
    std::reference_wrapper<const Material> material;

    // Construct an empty hit record
    inline HitRecord() noexcept
        : time(0), hit_point(point3::ZEROS), surface_normal(vec3::ZEROS),
          front_face(false), material(std::cref(_DummyMaterial::d)) {}

    // Set the correct orientation of the normal
    constexpr void set_face_normal(const Ray & r,
                                   const Vec3 & outward_normal) noexcept {
        front_face = r.direction.dot(outward_normal) < 0;
        surface_normal = front_face ? outward_normal : -outward_normal;
    }

    // Scatter the ray according to the hit material
    inline Material::ScatterType scatter(Ray & ray,
                                         Colour & ray_colour) const noexcept {
        return material.get().scatter(*this, ray, ray_colour);
    }
};

// Abstract interface of a hittable object
class Hittable {
public:
    // Check if a ray hits the hittable object.
    // If there is a hit, return a hit record
    virtual bool hit(const Ray & ray_in,
                     double tmin,
                     double tmax,
                     HitRecord & hit_record) const noexcept = 0;

    // Virtual destructor
    virtual ~Hittable() noexcept = default;
};

// A dynamic list of hittable objects
class HittableList : public Hittable,
                     protected std::vector<std::unique_ptr<Hittable>> {
public:
    // The inner value type
    using value_type = std::unique_ptr<Hittable>;
    // The inner container for the hittable objects
    using container = std::vector<value_type>;
    // Iterator type
    using const_iterator = container::const_iterator;

public:
    // Construct an empty hittable list
    inline HittableList() noexcept {}

    // Add an object inheriting from the Hittable class
    template <class T>
    requires std::is_convertible_v<T *, Hittable *>
    inline void add(const T & object) noexcept {
        push_back(std::make_unique<T>(object));
    }

    // Add an already allocated unique_ptr to the hittable list
    inline void add(std::unique_ptr<Hittable> object) noexcept {
        push_back(std::move(object));
    }

    // Hit method override
    virtual bool hit(const Ray & ray_in,
                     double tmin,
                     double tmax,
                     HitRecord & hit_record) const noexcept override;
};

#endif
