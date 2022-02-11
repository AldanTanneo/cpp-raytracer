#ifndef HITTABLE_HPP
#define HITTABLE_HPP

#include <memory>
#include <type_traits>
#include <vector>

// From src/include
#include <ray.hpp>
#include <utils/pdf.hpp>
#include <utils/vec3.hpp>

class HitRecord; // Forward declaration of HitRecord

// The returned structure when a material scatters an object
class ScatterRecord {
public:
    // The colour of the material
    Colour attenuation;
    // Wether the scattered ray is specular
    bool is_specular;
    // If it is specular, the direction of the new ray
    Vec3 specular_direction;
    // If it's diffuse, the corresponding PDF
    std::unique_ptr<Pdf> pdf;

    // Default constructor
    constexpr ScatterRecord() noexcept = default;
};

// Abstract interface of a material
class Material {
public:
    template <class T>
    constexpr static bool is_material = std::is_convertible_v<T *, Material *>;

    enum ScatterType { None = 0, Emit = 1, Bounce = 2 };

    // Define how a ray should interact with the material.
    // Modifies the scatter record object accordingly
    virtual ScatterType scatter(const HitRecord & hit_record,
                                const Ray & ray_in,
                                ScatterRecord & scatter) const noexcept = 0;

    // Samples the material along the newly emitted ray
    virtual double scattering_pdf(const HitRecord & hit_record,
                                  const Ray & scattered_ray) const noexcept {
        return 0.0;
    }

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
    scatter(const HitRecord & hit_record,
            const Ray & ray_in,
            ScatterRecord & scatter) const noexcept override {
        return ScatterType::None;
    }

    // Never scatter
    virtual double scattering_pdf(const HitRecord &,
                                  const Ray &) const noexcept override {
        return 0.0;
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
    inline Material::ScatterType
    scatter(const Ray & ray_in, ScatterRecord & scatter) const noexcept {
        return material.get().scatter(*this, ray_in, scatter);
    }
};

// Abstract interface of a hittable object
class Hittable {
public:
    // Check if a ray hits the hittable object.
    // If there is a hit, return a hit record
    virtual bool hit(const Ray & ray_in,
                     const double tmin,
                     const double tmax,
                     HitRecord & hit_record) const noexcept = 0;

    // Sample the hittable pdf along a direction
    virtual double pdf_value(const Point3 & origin,
                             const Vec3 & direction) const noexcept {
        return 0.0;
    }

    // Return a vector to a random point on the object's surface, visible from
    // the given origin
    virtual Vec3 random(const Vec3 & origin) const noexcept {
        return vec3::ZEROS;
    }

    // Virtual destructor
    virtual ~Hittable() noexcept = default;
};

// A dynamic list of hittable objects
class HittableList
    : public Hittable,
      protected std::vector<std::reference_wrapper<const Hittable>> {
public:
    // The inner value type
    using value_type = std::reference_wrapper<const Hittable>;
    // The inner container for the hittable objects
    using container = std::vector<value_type>;
    // Iterator type
    using const_iterator = container::const_iterator;

public:
    // Construct an empty hittable list
    inline HittableList() noexcept {}

    // Add an object inheriting from the Hittable class
    inline void add(const Hittable & object) noexcept {
        push_back(std::cref(object));
    }

    // Hit method override
    virtual bool hit(const Ray & ray_in,
                     const double tmin,
                     const double tmax,
                     HitRecord & hit_record) const noexcept override;

    // Virtual function override
    virtual double pdf_value(const Point3 & origin,
                             const Vec3 & direction) const noexcept override;

    // Virtual function override
    virtual Vec3 random(const Point3 & origin) const noexcept override;
};

// A PDF for sampling random points on hittable objects
class HittablePdf : public Pdf {
private:
    // The hittable object
    const Hittable & obj;
    // The origin from which the object is sampled
    const Point3 origin;

public:
    // Construct a new hittable PDF
    constexpr HittablePdf(const Hittable & obj, const Point3 & origin)
        : obj(obj), origin(origin) {}

    // Virtual function override
    virtual double value(const Vec3 & direction) const noexcept override {
        return obj.pdf_value(origin, direction);
    }

    // Virtual function override
    virtual Vec3 generate() const noexcept override {
        return obj.random(origin);
    }
};

#endif
