#ifndef VEC3_HPP
#define VEC3_HPP

#include <cmath>
#include <cstdint>
#include <ostream>

// From src/include
#include <utils.hpp>
#include <utils/rng.hpp>

// RGB colours
namespace colour {
    // RGB colour type. Uses 24-bits colour encoding for output and input.
    class Colour {
    private:
        constexpr static double RED_LUMINANCE = 0.2126;
        constexpr static double GREEN_LUMINANCE = 0.7152;
        constexpr static double BLUE_LUMINANCE = 0.0722;

    public:
        // RGB components
        double r, g, b;

        // Construct a colour from its hexadecimal representation
        constexpr Colour(uint32_t value) noexcept
            : r(((value >> 16) & 0xff) / 255.0),
              g(((value >> 8) & 0xff) / 255.0), b((value & 0xff) / 255.0) {}
        // Construct an empty (black) colour
        constexpr Colour() noexcept : r(0.0), g(0.0), b(0.0) {}
        // Construct a colour from its linear floating point RGB components
        constexpr Colour(double r, double g, double b) noexcept
            : r(r), g(g), b(b) {}

        // Construct a colour from its byte RGB components
        constexpr static Colour
        rgb(uint8_t red, uint8_t green, uint8_t blue) noexcept {
            return Colour(double(red) / 255.0, double(green) / 255.0,
                          double(blue) / 255.0);
        }
        // Gamma corrected red component
        inline uint8_t red() const noexcept {
            return static_cast<uint8_t>(utils::gamma_correction(r)
                                        * utils::MAX_COLOUR);
        }
        // Gamma corrected green component
        inline uint8_t green() const noexcept {
            return static_cast<uint8_t>(utils::gamma_correction(g)
                                        * utils::MAX_COLOUR);
        }
        // Gamma corrected blue component
        inline uint8_t blue() const noexcept {
            return static_cast<uint8_t>(utils::gamma_correction(b)
                                        * utils::MAX_COLOUR);
        }
        // Get the hexadecimal representation of the colour
        inline uint32_t to_hex() const noexcept {
            return (red() << 16) | (green() << 8) | blue();
        }
        // Get the luminance of the colour
        constexpr double luminance() const noexcept {
            return RED_LUMINANCE * r + GREEN_LUMINANCE * g + BLUE_LUMINANCE * b;
        }
        // Invert a colour
        constexpr Colour invert() const noexcept {
            return Colour(1.0 - r, 1.0 - g, 1.0 - b);
        }

        /// Addition
        // Add two colours componentwise
        constexpr Colour operator+(const Colour & other) const noexcept {
            return Colour(r + other.r, g + other.g, b + other.b);
        }
        // Add a scalar value to all RGB components
        constexpr Colour operator+(const double f) const noexcept {
            return Colour(r + f, g + f, b + f);
        }
        // Add a scalar value to all RGB components
        constexpr friend Colour operator+(const double f,
                                          const Colour & self) noexcept {
            return self + f;
        }
        // Add-assign a colour componentwise
        constexpr void operator+=(const Colour & other) noexcept {
            r += other.r;
            g += other.g;
            b += other.b;
        }
        // Add-assign a scalar value to all RGB components
        constexpr void operator+=(const double f) noexcept {
            r += f;
            g += f;
            b += f;
        }
        /// Subtraction
        // Substract two colours componentwise
        constexpr Colour operator-(const Colour & other) const noexcept {
            return Colour(r - other.r, g - other.g, b - other.b);
        }
        // Substract a scalar value from all RGB components
        constexpr Colour operator-(const double f) const noexcept {
            return Colour(r - f, g - f, b - f);
        }
        // Substract RGB components to a scalar value, returning a new colour
        constexpr friend Colour operator-(const double f,
                                          const Colour & self) noexcept {
            return Colour(f - self.r, f - self.g, f - self.b);
        }
        // Substract-assign a colour componentwise
        constexpr void operator-=(const Colour & other) noexcept {
            r -= other.r;
            g -= other.g;
            b -= other.b;
        }
        // Substract-assign a scalar value to all RGB components
        constexpr void operator-=(const double f) noexcept {
            r -= f;
            g -= f;
            b -= f;
        }
        /// Multiplication
        // Multiply two colours componentwise
        constexpr Colour operator*(const Colour & other) const noexcept {
            return Colour(r * other.r, g * other.g, b * other.b);
        }
        // Multiply a colour by a scalar value
        constexpr Colour operator*(const double f) const noexcept {
            return Colour(r * f, g * f, b * f);
        }
        // Multiply a colour by a scalar value
        constexpr friend Colour operator*(const double f,
                                          const Colour & self) noexcept {
            return self * f;
        }
        // Multiply-assign a colour componentwise
        constexpr void operator*=(const Colour & other) noexcept {
            r *= other.r;
            g *= other.g;
            b *= other.b;
        }
        // Multiply-assign a scalar value to all RGB components
        constexpr void operator*=(const double f) noexcept {
            r *= f;
            g *= f;
            b *= f;
        }
        /// Division
        // Divide two colours componentwise
        constexpr Colour operator/(const Colour & other) const noexcept {
            return Colour(r / other.r, g / other.g, b / other.b);
        }
        // Divide all RGB components by a scalar value
        constexpr Colour operator/(const double f) const noexcept {
            return Colour(r / f, g / f, b / f);
        }
        // Divide-assign a colour componentwise
        constexpr void operator/=(const Colour & other) noexcept {
            r /= other.r;
            g /= other.g;
            b /= other.b;
        }
        // Divide-assign all RGB components by a scalar value
        constexpr void operator/=(const double f) noexcept {
            r /= f;
            g /= f;
            b /= f;
        }

        // Clamp a colour to the [0.0, 1.0] range
        constexpr Colour clamp() const noexcept {
            return Colour(utils::clamp(r), utils::clamp(g), utils::clamp(b));
        }

        // Clamp a colour to the [a, b] range
        constexpr Colour clamp(const double a, const double b) const noexcept {
            return Colour(utils::clamp(r, a, b), utils::clamp(g, a, b),
                          utils::clamp(b, a, b));
        }

        // Compare two colours for equality
        inline bool operator==(const Colour & other) const noexcept {
            return red() == other.red() && green() == other.green()
                   && blue() == other.blue();
        }

        // Compare two colours for inequality
        inline bool operator!=(const Colour & other) const noexcept {
            return red() != other.red() || green() != other.green()
                   || blue() != other.blue();
        }

        // Print the color for debugging purposes
        template <class charT, class charTraits = std::char_traits<charT>>
        inline friend std::basic_ostream<charT, charTraits> &
        operator<<(std::basic_ostream<charT, charTraits> & os,
                   const Colour & self) {
            return os << "Colour(" << self.r << ", " << self.g << ", " << self.b
                      << ")";
        }

        // Output the colour to a stream
        template <class charT, class charTraits = std::char_traits<charT>>
        inline void write(std::basic_ostream<charT, charTraits> & os) const {
            os << red() << green() << blue();
        }
    };

    // For the americans
    using Color = Colour;

} // namespace colour

// Namespace alias
namespace color = colour;

// External declarations
using color::Color;
using colour::Colour;

/// Constants
namespace colour {
    // Pure red colour
    constexpr Colour RED(0xff0000);

    // Pure green colour
    constexpr Colour GREEN(0x00ff00);

    // Pure blue colour
    constexpr Colour BLUE(0x0000ff);

    // Pure yellow colour
    constexpr Colour YELLOW(0xffff00);

    // Pure cyan colour
    constexpr Colour CYAN(0x00ffff);

    // Pure magenta colour
    constexpr Colour MAGENTA(0xff00ff);

    // Pure black colour
    constexpr Colour BLACK(0x000000);

    // Pure white colour
    constexpr Colour WHITE(0xffffff);
} // namespace colour

// Three dimensional vectors
namespace vec3 {
    // A three dimensional vector class
    class Vec3 {
    public:
        // Vector components
        double x, y, z;

        /// Constructors
        // Default Constructor
        constexpr Vec3() noexcept : x(0), y(0), z(0) {}
        // Two dimensional constructor
        constexpr Vec3(double x, double y) noexcept : x(x), y(y), z(0) {}
        // Three dimensional constructor
        constexpr Vec3(double x, double y, double z) noexcept
            : x(x), y(y), z(z) {}
        // Convert a Vec3 to a Colour
        explicit constexpr operator Colour() noexcept {
            return Colour(x, y, z);
        }

        /// Operator overloading
        // Get the opposite of a vector
        constexpr Vec3 operator-() const noexcept { return Vec3(-x, -y, -z); }
        // Unary + operator
        constexpr Vec3 operator+() const noexcept { return *this; }
        /// Addition
        // Add two vectors componentwise
        constexpr Vec3 operator+(const Vec3 & other) const noexcept {
            return Vec3(x + other.x, y + other.y, z + other.z);
        }
        // Add a scalar value to all components of a vector
        constexpr Vec3 operator+(const double f) const noexcept {
            return Vec3(x + f, y + f, z + f);
        }
        // Add a scalar value to all components of a vector
        constexpr friend Vec3 operator+(const double f,
                                        const Vec3 & self) noexcept {
            return self + f;
        }
        // Add-assign a vector componentwise
        constexpr void operator+=(const Vec3 & other) noexcept {
            x += other.x;
            y += other.y;
            z += other.z;
        }
        // Add-assign a scalar value to all components of the vector
        constexpr void operator+=(const double f) noexcept {
            x += f;
            y += f;
            z += f;
        }
        /// Subtraction
        // Substract two vectors componentwise
        constexpr Vec3 operator-(const Vec3 & other) const noexcept {
            return Vec3(x - other.x, y - other.y, z - other.z);
        }
        // Substract a scalar value from all components of a vector
        constexpr Vec3 operator-(const double f) const noexcept {
            return Vec3(x - f, y - f, z - f);
        }
        // Substract a vector from a scalar value, returning a new vector
        constexpr friend Vec3 operator-(const double f,
                                        const Vec3 & self) noexcept {
            return Vec3(f - self.x, f - self.y, f - self.z);
        }
        // Substract-assign a vector componentwise
        constexpr void operator-=(const Vec3 & other) noexcept {
            x -= other.x;
            y -= other.y;
            z -= other.z;
        }
        // Substract-assign a scalar value to all components of the vector
        constexpr void operator-=(const double f) noexcept {
            x -= f;
            y -= f;
            z -= f;
        }
        /// Multiplication
        // Multiply two vectors componentwise
        constexpr Vec3 operator*(const Vec3 & other) const noexcept {
            return Vec3(x * other.x, y * other.y, z * other.z);
        }
        // Multiply a vector by a scalar value
        constexpr Vec3 operator*(const double f) const noexcept {
            return Vec3(x * f, y * f, z * f);
        }
        // Multiply a vector by a scalar value
        constexpr friend Vec3 operator*(const double f,
                                        const Vec3 & self) noexcept {
            return self * f;
        }
        // Multiply-assign a vector componentwise
        constexpr void operator*=(const Vec3 & other) noexcept {
            x *= other.x;
            y *= other.y;
            z *= other.z;
        }
        // Multiply-assign a scalar value to all components of the vector
        constexpr void operator*=(const double f) noexcept {
            x *= f;
            y *= f;
            z *= f;
        }
        /// Division
        // Divide two vectors componentwise
        constexpr Vec3 operator/(const Vec3 & other) const noexcept {
            return Vec3(x / other.x, y / other.y, z / other.z);
        }
        // Divide a vector by a scalar value
        constexpr Vec3 operator/(const double f) const noexcept {
            return Vec3(x / f, y / f, z / f);
        }
        // Divide a scalar by a vector, returning a new vector
        constexpr friend Vec3 operator/(const double f,
                                        const Vec3 & self) noexcept {
            return Vec3(f / self.x, f / self.y, f / self.z);
        }
        // Divide-assign a vector componentwise
        constexpr void operator/=(const Vec3 & other) noexcept {
            x /= other.x;
            y /= other.y;
            z /= other.z;
        }
        // Divide-assign a scalar value to all components of the vector
        constexpr void operator/=(const double f) noexcept {
            x /= f;
            y /= f;
            z /= f;
        }

        /// Norm and distance
        // Squared norm of the vector
        constexpr double squared_norm() const noexcept {
            return x * x + y * y + z * z;
        }
        // Norm of the vector
        inline double norm() const noexcept { return sqrt(squared_norm()); }
        // Normalize the vector
        inline Vec3 unit_vector() const noexcept { return *this / norm(); }
        // Normalize the vector. Modifies the original vector.
        inline void normalize() noexcept { *this /= norm(); }
        // Get the distance between two points
        inline double distance(const Vec3 & other) const noexcept {
            return (other - *this).norm();
        }
        // Check if the vector is near zero
        inline bool near_zero() const noexcept {
            return fabs(x) < utils::EPSILON && fabs(y) < utils::EPSILON
                   && fabs(z) < utils::EPSILON;
        }

        /// Geometric operations
        // Dot product
        constexpr double dot(const Vec3 & other) const noexcept {
            return x * other.x + y * other.y + z * other.z;
        }
        // Cross product
        constexpr Vec3 cross(const Vec3 & other) const noexcept {
            return Vec3(y * other.z - z * other.y, z * other.x - x * other.z,
                        x * other.y - y * other.x);
        }
        // Reflect a vector against a plane defined by its normal
        constexpr Vec3 reflect(const Vec3 & normal) const noexcept {
            return *this - 2.0 * dot(normal) * normal;
        }
        // Refract a vector against a plane defined by its normal, with a given
        // refraction ratio.
        inline Vec3 refract(const Vec3 & normal,
                            double refraction_ratio) const noexcept {
            double rcos_theta = -dot(normal);
            Vec3 orth_out = refraction_ratio * (*this + normal * rcos_theta);
            Vec3 parr_out =
                -sqrt(squared_norm() - orth_out.squared_norm()) * normal;
            return orth_out + parr_out;
        }

        /// Random vector functions
        // Returns a random vector with coordinates in range [-1, 1)
        inline static Vec3 random() noexcept {
            return 2.0 * Vec3(rng::gen(), rng::gen(), rng::gen()) - 1.0;
        }
        // Returns a random vector in the unit sphere
        inline static Vec3 random_in_unit_sphere() noexcept {
            Vec3 res = Vec3::random();
            while (res.squared_norm() > 1.0) {
                res = Vec3::random();
            }
            return res;
        }
        // Returns a random unit vector
        inline static Vec3 random_unit_vector() noexcept {
            return Vec3::random_in_unit_sphere().unit_vector();
        }
        // Returns a random vector in the hemisphere defined by the given vector
        inline static Vec3 random_in_hemisphere(const Vec3 & u) noexcept {
            Vec3 res = Vec3::random();
            if (res.dot(u) < 0.0) {
                return -res;
            } else {
                return res;
            }
        }

        inline static Vec3 random_cosine_direction() noexcept {
            auto r1 = rng::gen();
            auto r2 = rng::gen();
            auto z = sqrt(1 - r2);
            auto r = sqrt(r2);

            auto phi = utils::TAU * r1;
            auto x = cos(phi) * r;
            auto y = sin(phi) * r;

            return Vec3(x, y, z);
        }

        // Print the vector for debugging purposes
        template <class charT, class charTraits = std::char_traits<charT>>
        inline friend std::basic_ostream<charT, charTraits> &
        operator<<(std::basic_ostream<charT, charTraits> & os,
                   const Vec3 & self) {
            return os << "Vec3(" << self.x << ", " << self.y << ", " << self.z
                      << ")";
        }
    };

    // Type alias
    using Point3 = Vec3;

}; // namespace vec3

// Namespace alias
namespace point3 = vec3;

// External declarations
using point3::Point3;
using vec3::Vec3;

/// Constants
namespace vec3 {
    // Null vector
    constexpr Vec3 ZEROS(0, 0, 0);
    // Vector of ones
    constexpr Vec3 ONES(1, 1, 1);
    // Unit x vector
    constexpr Vec3 X(1, 0, 0);
    // Unit y vector
    constexpr Vec3 Y(0, 1, 0);
    // Unit z vector
    constexpr Vec3 Z(0, 0, 1);
} // namespace vec3

#endif
