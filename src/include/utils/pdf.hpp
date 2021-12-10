#ifndef PDF_HPP
#define PDF_HPP

// From src/include
#include <utils/orthonormal_bases.hpp>
#include <utils/vec3.hpp>

// Probability Density Function
class Pdf {
public:
    /* Get the value of the PDF in the given direction */
    virtual double value(const Vec3 & direction) const noexcept = 0;
    /* Sample the pdf */
    virtual Vec3 generate() const noexcept = 0;

    virtual ~Pdf() noexcept = default;
};

// Construct a PDF from two PDFs by averaging them
class MixturePdf : public Pdf {
private:
    const Pdf & alpha;
    const Pdf & beta;

public:
    constexpr MixturePdf(const Pdf & alpha, const Pdf & beta) noexcept
        : alpha(alpha), beta(beta) {}

    virtual double value(const Vec3 & direction) const noexcept override;

    virtual Vec3 generate() const noexcept override;
};

// Construct a PDF in cos(theta) along a normal (diffuse materials)
class CosinePdf : public Pdf {
private:
    // Orthonormal basis
    const Onb uvw;

public:
    inline CosinePdf(const Vec3 n) noexcept : uvw(Onb::from_unit_normal(n)) {}

    virtual double value(const Vec3 & direction) const noexcept override;

    virtual Vec3 generate() const noexcept override;
};

// Construct a PDF following the Oren-Nayar microfacet model
// (https://pbr-book.org/3ed-2018/Reflection_Models/Microfacet_Models#OrenndashNayarDiffuseReflection)
class OrenNayar : public Pdf {
private:
    // Orthonormal basis;
    const Onb uvw;
    // PDF roughness parameters
    const double A, B;
    // Incoming ray direction parameters
    double cos_theta_in, sin_theta_in, cos_phi_in, sin_phi_in;

public:
    inline OrenNayar(const Vec3 n,
                     const Vec3 unit_direction,
                     const double A,
                     const double B) noexcept
        : uvw(Onb::from_unit_normal(n)), A(A), B(B) {
        cos_theta_in = unit_direction.dot(uvw.w);
        sin_theta_in = sqrt(1.0 - cos_theta_in * cos_theta_in);
        const Vec3 in_dir_tangent = unit_direction - cos_theta_in * uvw.w;
        cos_phi_in = in_dir_tangent.dot(uvw.u) / sin_theta_in;
        sin_phi_in = in_dir_tangent.dot(uvw.v) / sin_theta_in;
    }

    virtual double value(const Vec3 & direction) const noexcept override;

    virtual Vec3 generate() const noexcept override;
};

#endif
