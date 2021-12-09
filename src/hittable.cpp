#include <memory>

// From src/include
#include <hittable.hpp>
#include <ray.hpp>
#include <utils/vec3.hpp>

const _DummyMaterial _DummyMaterial::d;

bool HittableList::hit(const Ray & ray_in,
                       double tmin,
                       double tmax,
                       HitRecord & hit_record) const noexcept {
    bool hit = false;
    for (const value_type & obj : *this) {
        if (obj->hit(ray_in, tmin, tmax, hit_record)) {
            hit = true;
            tmax = hit_record.time;
        }
    }
    return hit;
}

double HittableList::pdf_value(const Point3 & origin,
                               const Vec3 & direction) const noexcept {
    auto weight = 1.0 / size();
    auto sum = 0.0;

    for (const value_type & obj : *this)
        sum += weight * obj->pdf_value(origin, direction);

    return sum;
}

// Virtual function override
Vec3 HittableList::random(const Point3 & origin) const noexcept {
    return at(rng::gen_u64() % size())->random(origin);
}
