#include <memory>
#include <omp.h>

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

    for (const Hittable & obj : *this) {
        if (obj.hit(ray_in, tmin, tmax, hit_record)) {
            hit = true;
            tmax = hit_record.time;
        }
    }

    return hit;
}

double HittableList::pdf_value(const Point3 & origin,
                               const Vec3 & direction) const noexcept {
    double sum = 0.0;
    for (const Hittable & obj : *this) {
        sum += obj.pdf_value(origin, direction);
    }
    return sum / size();
}

Vec3 HittableList::random(const Point3 & origin) const noexcept {
    return operator[](rng::gen_u64() % size()).get().random(origin);
}
