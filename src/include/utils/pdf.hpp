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

protected:
    virtual ~Pdf() noexcept = default;
};

// Construct a PDF from two PDFs by averaging them
class MixturePdf : public Pdf {
private:
    const Pdf * alpha;
    const Pdf * beta;

public:
    constexpr MixturePdf(const Pdf * alpha, const Pdf * beta) noexcept
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

#endif
