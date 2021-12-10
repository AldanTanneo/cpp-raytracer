#include <cmath>
#include <utils/pdf.hpp>

double MixturePdf::value(const Vec3 & direction) const noexcept {
    return 0.5 * alpha.value(direction) + 0.5 * beta.value(direction);
}

Vec3 MixturePdf::generate() const noexcept {
    if (rng::gen() < 0.5) {
        return alpha.generate();
    } else {
        return beta.generate();
    }
}

double CosinePdf::value(const Vec3 & direction) const noexcept {
    const double cos_theta = direction.unit_vector().dot(uvw.w);
    return cos_theta < utils::EPSILON ? 0.0 : cos_theta / utils::PI;
}

Vec3 CosinePdf::generate() const noexcept {
    return uvw.local(Vec3::random_cosine_direction());
}

double OrenNayar::value(const Vec3 & direction) const noexcept {
    const double cos_theta_out = direction.unit_vector().dot(uvw.w);
    const double sin_theta_out = sqrt(1.0 - cos_theta_out * cos_theta_out);
    const Vec3 out_dir_tangent = direction - cos_theta_out * uvw.w;

    double max_cos = 0;
    if (sin_theta_in > utils::EPSILON && sin_theta_out > utils::EPSILON) {
        const double cos_phi_out = out_dir_tangent.dot(uvw.u) / sin_theta_out;
        const double sin_phi_out = out_dir_tangent.dot(uvw.v) / sin_theta_out;
        max_cos = fmax(0, cos_phi_in * cos_phi_out + sin_phi_in * sin_phi_out);
    }

    double sin_alpha, tan_beta;
    if (fabs(cos_theta_in) > fabs(cos_theta_out)) {
        sin_alpha = sin_theta_out;
        tan_beta = sin_theta_in / fabs(cos_theta_in);
    } else {
        sin_alpha = sin_theta_in;
        tan_beta = sin_theta_out / fabs(cos_theta_out);
    }

    return (A + B * max_cos * sin_alpha * tan_beta) / utils::PI;
}

Vec3 OrenNayar::generate() const noexcept {
    return uvw.local(Vec3::random_cosine_direction());
}
