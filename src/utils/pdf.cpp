#include <utils/pdf.hpp>

double CosinePdf::value(const Vec3 & direction) const noexcept {
    const double cos_theta = direction.dot(uvw.w);
    return cos_theta < utils::EPSILON ? 0.0 : cos_theta / utils::PI;
}

Vec3 CosinePdf::generate() const noexcept {
    return uvw.local(Vec3::random_cosine_direction()).unit_vector();
}

double MixturePdf::value(const Vec3 & direction) const noexcept {
    return 0.5 * alpha->value(direction) + 0.5 * beta->value(direction);
}

Vec3 MixturePdf::generate() const noexcept {
    if (rng::gen() < 0.5) {
        return alpha->generate();
    } else {
        return beta->generate();
    }
}
