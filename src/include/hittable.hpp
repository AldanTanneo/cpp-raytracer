#ifndef HITTABLE_HPP
#define HITTABLE_HPP

#include <memory>
#include <vector>

// From src/include
#include <ray.hpp>
#include <utils/vec3.hpp>

/* The returned structure if a ray hits an object */
struct HitRecord {
    double time;
    /* The point of intersection between the ray and the object */
    Point3 hit_point;
    /* The normal of the object surface at the intersection point */
    Vec3 surface_normal;
    /* The material of the object surface */
    // Material mat;

    /* Construct an empty hit record */
    constexpr HitRecord() noexcept : time(0.0), hit_point(point3::ZEROS), surface_normal(point3::ZEROS) {}
};

/* Abstract class of a hittable object */
class Hittable {
  public:
    /* Check if a ray hits the hittable object. If there is a hit, return a hit record */
    virtual bool hit(const Ray & ray_in, double tmin, double tmax, HitRecord & hit_record) const noexcept = 0;

    /* Virtual destructor */
    virtual ~Hittable() {}
};

/* A dynamic list of hittable objects */
class HittableList : public Hittable {
  private:
    /* The inner container */
    std::vector<std::unique_ptr<Hittable>> objects;

  public:
    inline HittableList() noexcept : objects(std::vector<std::unique_ptr<Hittable>>()) {}

    const std::vector<std::unique_ptr<Hittable>> & iter() const noexcept {
        return objects;
    }

    template <class T>
    void add(T object) noexcept {
        objects.push_back(std::make_unique<T>(object));
    }

    const std::unique_ptr<Hittable> & operator[](int index) const {
        return objects[index];
    }

    virtual bool hit(const Ray & ray_in, double tmin, double tmax, HitRecord & hit_record) const noexcept override;
};

#endif
