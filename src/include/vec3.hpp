#ifndef VEC3_HPP
#define VEC3_HPP

#include <cmath>
#include <ostream>

namespace utils {
/* Maximum floating point color value. Used for conversion between floating point and integer color. */
constexpr double MAX_COLOUR = 255.999999;
/* Common epsilon for floating point uses */
constexpr double EPSILON = 1e-9;
/* Geometric PI constant */
constexpr double PI = 3.14159265358979323846264338327950288;

template <class T>
constexpr T to_radians(T value) noexcept {
    static_assert(std::is_floating_point<T>::value, "T must be a floating-point type!");
    return value * static_cast<T>(PI) / 180.0;
}

/* Clamps a floating point value to the [0.0, 1.0] range */
template <class T>
constexpr T clamp(const T value) noexcept {
    static_assert(std::is_floating_point<T>::value, "T must be a floating-point type!");
    if (value < 0.0)
        return static_cast<T>(0.0);
    if (value > 1.0)
        return static_cast<T>(1.0);
    return value;
}

/* Clamps an ordered value to the [min, max] range. */
template <class T>
constexpr T clamp(const T value, const T min, const T max) noexcept {
    if (value < min) {
        return min;
    }
    if (value > max) {
        return max;
    }
    return value;
}

} // namespace utils

/* Three dimensional vectors */
namespace vec3 {
/* A three dimensional vector class */
class Vec3 {
  public:
    /* Vector components */
    double x, y, z;

    /// Constructors
    /* Default Constructor */
    constexpr Vec3() noexcept : x(0), y(0), z(0) {}
    /* Two dimensional constructor */
    explicit constexpr Vec3(double x, double y) noexcept : x(x), y(y), z(0) {}
    /* Three dimensional constructor */
    explicit constexpr Vec3(double x, double y, double z) noexcept : x(x), y(y), z(z) {}

    /// Operator overloading
    /* Get the opposite of a vector */
    constexpr Vec3 operator-() const noexcept {
        return Vec3(-x, -y, -z);
    }
    /* Unary + operator */
    constexpr Vec3 operator+() const noexcept {
        return *this;
    }
    /// Addition
    /* Add two vectors componentwise */
    constexpr Vec3 operator+(const Vec3 & other) const noexcept {
        return Vec3(x + other.x, y + other.y, z + other.z);
    }
    /* Add a scalar value to all components of a vector */
    constexpr Vec3 operator+(const double f) const noexcept {
        return Vec3(x + f, y + f, z + f);
    }
    /* Add a scalar value to all components of a vector */
    constexpr friend Vec3 operator+(const double f, const Vec3 & self) noexcept {
        return self + f;
    }
    /* Add-assign a vector componentwise */
    constexpr void operator+=(const Vec3 & other) noexcept {
        x += other.x;
        y += other.y;
        z += other.z;
    }
    /* Add-assign a scalar value to all components of the vector */
    constexpr void operator+=(const double f) noexcept {
        x += f;
        y += f;
        z += f;
    }
    /// Subtraction
    /* Substract two vectors componentwise */
    constexpr Vec3 operator-(const Vec3 & other) const noexcept {
        return Vec3(x - other.x, y - other.y, z - other.z);
    }
    /* Substract a scalar value from all components of a vector */
    constexpr Vec3 operator-(const double f) const noexcept {
        return Vec3(x - f, y - f, z - f);
    }
    /* Substract a vector from a scalar value, returning a new vector */
    constexpr friend Vec3 operator-(const double f, const Vec3 & self) noexcept {
        return Vec3(f - self.x, f - self.y, f - self.z);
    }
    /* Substract-assign a vector componentwise */
    constexpr void operator-=(const Vec3 & other) noexcept {
        x -= other.x;
        y -= other.y;
        z -= other.z;
    }
    /* Substract-assign a scalar value to all components of the vector */
    constexpr void operator-=(const double f) noexcept {
        x -= f;
        y -= f;
        z -= f;
    }
    /// Multiplication
    /* Multiply two vectors componentwise */
    constexpr Vec3 operator*(const Vec3 & other) const noexcept {
        return Vec3(x * other.x, y * other.y, z * other.z);
    }
    /* Multiply a vector by a scalar value */
    constexpr Vec3 operator*(const double f) const noexcept {
        return Vec3(x * f, y * f, z * f);
    }
    /* Multiply a vector by a scalar value */
    constexpr friend Vec3 operator*(const double f, const Vec3 & self) noexcept {
        return self * f;
    }
    /* Multiply-assign a vector componentwise */
    constexpr void operator*=(const Vec3 & other) noexcept {
        x *= other.x;
        y *= other.y;
        z *= other.z;
    }
    /* Multiply-assign a scalar value to all components of the vector */
    constexpr void operator*=(const double f) noexcept {
        x *= f;
        y *= f;
        z *= f;
    }
    /// Division
    /* Divide two vectors componentwise */
    constexpr Vec3 operator/(const Vec3 & other) const {
        return Vec3(x / other.x, y / other.y, z / other.z);
    }
    /* Divide a vector by a scalar value */
    constexpr Vec3 operator/(const double f) const {
        return Vec3(x / f, y / f, z / f);
    }
    /* Divide a scalar by a vector, returning a new vector */
    constexpr friend Vec3 operator/(const double f, const Vec3 & self) {
        return Vec3(f / self.x, f / self.y, f / self.z);
    }
    /* Divide-assign a vector componentwise */
    constexpr void operator/=(const Vec3 & other) noexcept {
        x /= other.x;
        y /= other.y;
        z /= other.z;
    }
    /* Divide-assign a scalar value to all components of the vector */
    constexpr void operator/=(const double f) noexcept {
        x /= f;
        y /= f;
        z /= f;
    }

    /// Geometric operations
    /* Dot product */
    constexpr double dot(const Vec3 & other) const noexcept {
        return x * other.x + y * other.y + z * other.z;
    }
    /* Cross product */
    constexpr Vec3 cross(const Vec3 & other) const noexcept {
        return Vec3(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
    }
    /* Reflect a vector against a plane defined by its normal */
    constexpr Vec3 reflect(const Vec3 & normal) const noexcept {
        const Vec3 normal_component = dot(normal) * normal;
        return *this - 2 * normal_component;
    }
    /* Refract a vector against a plane defined by its normal, with a given refraction ratio.
    The current vector and the normal MUST BE unit vectors */
    inline Vec3 refract(const Vec3 & normal, double refraction_ratio) const noexcept {
        double cos_theta = -dot(normal);
        Vec3 orth_out = refraction_ratio * (*this + normal * cos_theta);
        Vec3 parr_out = -sqrt(abs((1 - orth_out.squared_norm()))) * normal;
        return orth_out + parr_out;
    }
    /* Squared norm of the vector */
    constexpr double squared_norm() const noexcept {
        return x * x + y * y + z * z;
    }
    /* Norm of the vector */
    inline double norm() const noexcept {
        return sqrt(squared_norm());
    }
    /* Normalize the vector */
    inline Vec3 unit_vector() const {
        return *this / norm();
    }
    /* Normalize the vector. Modifies the original vector. */
    inline void normalize() {
        *this /= norm();
    }
    /* Get the distance between two points */
    inline double distance(const Vec3 & other) const noexcept {
        return (other - *this).norm();
    }

    /* Print the vector for debugging purposes */
    template <class charT, class charTraits = std::char_traits<charT>>
    inline friend std::basic_ostream<charT, charTraits> & operator<<(std::basic_ostream<charT, charTraits> & os, const Vec3 & self) {
        return os << "Vec3(" << self.x << ", " << self.y << ", " << self.z << ")";
    }
};

/* Type alias */
using Point3 = Vec3;

}; // namespace vec3

/* Namespace alias */
namespace point3 = vec3;

/* External declarations */
using point3::Point3;
using vec3::Vec3;

/// Constants
namespace vec3 {
/* Null vector */
constexpr Vec3 ZEROS(0, 0, 0);
/* Vector of ones */
constexpr Vec3 ONES(1, 1, 1);
/* Unit x vector */
constexpr Vec3 X(1, 0, 0);
/* Unit y vector */
constexpr Vec3 Y(0, 1, 0);
/* Unit z vector */
constexpr Vec3 Z(0, 0, 1);
} // namespace vec3

/* RGB colours */
namespace colour {
/* RGB colour type. Uses 24-bits colour encoding for output and input. */
class Colour {
  public:
    /* RGB components */
    double r, g, b;

    /* Construct a colour from its hexadecimal representation */
    explicit constexpr Colour(uint32_t value)
        : r(((value >> 16) & 0xff) / 255.0),
          g(((value >> 8) & 0xff) / 255.0),
          b((value & 0xff) / 255.0) {}
    /* Construct a colour from its normalized floating point RGB components */
    explicit constexpr Colour(double r, double g, double b) : r(r), g(g), b(b) {}
    /* Convert a colour to a 3D vector */
    explicit constexpr operator Vec3() {
        return Vec3(r, g, b);
    }

    /* Construct a colour from its byte RGB components */
    constexpr static Colour rgb(uint8_t red, uint8_t green, uint8_t blue) {
        return Colour(double(red) / 255.0, double(green) / 255.0, double(blue) / 255.0);
    }
    /* Red component */
    constexpr uint8_t red() const noexcept {
        return static_cast<uint8_t>(utils::clamp(r) * utils::MAX_COLOUR);
    }
    /* Green component */
    constexpr uint8_t green() const noexcept {
        return static_cast<uint8_t>(utils::clamp(g) * utils::MAX_COLOUR);
    }
    /* Blue component */
    constexpr uint8_t blue() const noexcept {
        return static_cast<uint8_t>(utils::clamp(b) * utils::MAX_COLOUR);
    }
    /* Get the hexadecimal representation of the colour */
    constexpr uint32_t to_hex() const noexcept {
        return (red() << 16) | (green() << 8) | blue();
    }
    /* Invert a colour */
    constexpr Colour invert() const noexcept {
        return Colour(1.0 - r, 1.0 - g, 1.0 - b);
    }

    /// Addition
    /* Add two colours componentwise */
    constexpr Colour operator+(const Colour & other) const noexcept {
        return Colour(r + other.r, g + other.g, b + other.b);
    }
    /* Add a scalar value to all RGB components */
    constexpr Colour operator+(const double f) const noexcept {
        return Colour(r + f, g + f, b + f);
    }
    /* Add a scalar value to all RGB components */
    constexpr friend Colour operator+(const double f, const Colour & self) noexcept {
        return self + f;
    }
    /* Add-assign a colour componentwise */
    constexpr void operator+=(const Colour & other) noexcept {
        r += other.r;
        g += other.g;
        b += other.b;
    }
    /* Add-assign a scalar value to all RGB components */
    constexpr void operator+=(const double f) noexcept {
        r += f;
        g += f;
        b += f;
    }
    /// Subtraction
    /* Substract two colours componentwise */
    constexpr Colour operator-(const Colour & other) const noexcept {
        return Colour(r - other.r, g - other.g, b - other.b);
    }
    /* Substract a scalar value from all RGB components */
    constexpr Colour operator-(const double f) const noexcept {
        return Colour(r - f, g - f, b - f);
    }
    /* Substract RGB components to a scalar value, returning a new colour */
    constexpr friend Colour operator-(const double f, const Colour & self) noexcept {
        return Colour(f - self.r, f - self.g, f - self.b);
    }
    /* Substract-assign a colour componentwise */
    constexpr void operator-=(const Colour & other) noexcept {
        r -= other.r;
        g -= other.g;
        b -= other.b;
    }
    /* Substract-assign a scalar value to all RGB components */
    constexpr void operator-=(const double f) noexcept {
        r -= f;
        g -= f;
        b -= f;
    }
    /// Multiplication
    /* Multiply two colours componentwise */
    constexpr Colour operator*(const Colour & other) const noexcept {
        return Colour(r * other.r, g * other.g, b * other.b);
    }
    /* Multiply a colour by a scalar value */
    constexpr Colour operator*(const double f) const noexcept {
        return Colour(r * f, g * f, b * f);
    }
    /* Multiply a colour by a scalar value */
    constexpr friend Colour operator*(const double f, const Colour & self) noexcept {
        return self * f;
    }
    /* Multiply-assign a colour componentwise */
    constexpr void operator*=(const Colour & other) noexcept {
        r *= other.r;
        g *= other.g;
        b *= other.b;
    }
    /* Multiply-assign a scalar value to all RGB components */
    constexpr void operator*=(const double f) noexcept {
        r *= f;
        g *= f;
        b *= f;
    }
    /// Division
    /* Divide two colours componentwise */
    constexpr Colour operator/(const Colour & other) const noexcept {
        return Colour(r / other.r, g / other.g, b / other.b);
    }
    /* Divide all RGB components by a scalar value */
    constexpr Colour operator/(const double f) const noexcept {
        return Colour(r / f, g / f, b / f);
    }
    /* Divide-assign a colour componentwise */
    constexpr void operator/=(const Colour & other) noexcept {
        r /= other.r;
        g /= other.g;
        b /= other.b;
    }
    /* Divide-assign all RGB components by a scalar value */
    constexpr void operator/=(const double f) noexcept {
        r /= f;
        g /= f;
        b /= f;
    }

    /* Compare two colours for equality */
    constexpr bool operator==(const Colour & other) const noexcept {
        return red() == other.red() && green() == other.green() && blue() == other.blue();
    }

    /* Compare two colours for inequality */
    constexpr bool operator!=(const Colour & other) const noexcept {
        return red() != other.red() || green() != other.green() || blue() != other.blue();
    }

    /* Print the color for debugging purposes */
    template <class charT, class charTraits = std::char_traits<charT>>
    inline friend std::basic_ostream<charT, charTraits> & operator<<(std::basic_ostream<charT, charTraits> & os, const Colour & self) {
        return os << "Colour(" << self.r << ", " << self.g << ", " << self.b << ")";
    }

    /* Output the colour to a stream */
    template <class charT, class charTraits = std::char_traits<charT>>
    inline void write(std::basic_ostream<charT, charTraits> & os) const {
        os << red() << green() << blue();
    }
};

/* For the americans */
using Color = Colour;

} // namespace colour

/* Namespace alias */
namespace color = colour;

/* External declarations */
using color::Color;
using colour::Colour;

/// Constants
namespace colour {
/* Pure red colour */
constexpr Colour RED(0xff0000);

/* Pure green colour */
constexpr Colour GREEN(0x00ff00);

/* Pure blue colour */
constexpr Colour BLUE(0x0000ff);

/* Pure yellow colour */
constexpr Colour YELLOW(0xffff00);

/* Pure cyan colour */
constexpr Colour CYAN(0x00ffff);

/* Pure magenta colour */
constexpr Colour MAGENTA(0xff00ff);

/* Pure black colour */
constexpr Colour BLACK(0x000000);

/* Pure white colour */
constexpr Colour WHITE(0xffffff);
} // namespace colour

#endif
