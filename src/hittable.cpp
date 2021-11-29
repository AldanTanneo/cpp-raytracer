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
    for (const value_type & obj : objects) {
        if (obj->hit(ray_in, tmin, tmax, hit_record)) {
            hit = true;
            tmax = hit_record.time;
        }
    }
    return hit;
}
