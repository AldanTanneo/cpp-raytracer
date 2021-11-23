#ifndef VEC3_HPP
#define VEC3_HPP

#include <cmath>
#include <ostream>


namespace utils {

/* Maximum floating point color value. Used for conversion between floating point and integer color. */
constexpr float MAX_COLOUR = 255.999f;

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

} // namespace utils

/* Three dimensional vectors */
namespace vec3 {
/* A three dimensional vector class */
template <class T>
struct Vec3 {
    static_assert(std::is_floating_point<T>::value, "T must be a floating-point type!");
    /* Vector components */
    T x, y, z;

    /// Constants
    /* Null vector */
    constexpr static Vec3 ZEROS() noexcept { return Vec3(0, 0, 0); }
    /* Vector of ones */
    constexpr static Vec3 ONES() noexcept { return Vec3(1, 1, 1); }
    /* Unit x vector */
    constexpr static Vec3 X() noexcept { return Vec3(1, 0, 0); }
    /* Unit y vector */
    constexpr static Vec3 Y() noexcept { return Vec3(0, 1, 0); }
    /* Unit z vector */
    constexpr static Vec3 Z() noexcept { return Vec3(0, 0, 1); }

    /// Constructors
    /* Default Constructor */
    constexpr Vec3() noexcept : x(0), y(0), z(0) {}
    /* Two dimensional constructor */
    constexpr Vec3(T x, T y) noexcept : x(x), y(y), z(0) {}
    /* Three dimensional constructor */
    constexpr Vec3(T x, T y, T z) noexcept : x(x), y(y), z(z) {}

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
    constexpr Vec3 operator+(const T f) const noexcept {
        return Vec3(x + f, y + f, z + f);
    }
    /* Add a scalar value to all components of a vector */
    constexpr friend Vec3 operator+(const T f, const Vec3 & self) noexcept {
        return self + f;
    }
    /// Subtraction
    /* Substract two vectors componentwise */
    constexpr Vec3 operator-(const Vec3 & other) const noexcept {
        return Vec3(x - other.x, y - other.y, z - other.z);
    }
    /* Substract a scalar value from all components of a vector */
    constexpr Vec3 operator-(const T f) const noexcept {
        return Vec3(x - f, y - f, z - f);
    }
    /* Substract a vector from a scalar value, returning a new vector */
    constexpr friend Vec3 operator-(const T f, const Vec3 & self) noexcept {
        return Vec3(f - self.x, f - self.y, f - self.z);
    }
    /// Multiplication
    /* Multiply two vectors componentwise */
    constexpr Vec3 operator*(const Vec3 & other) const noexcept {
        return Vec3(x * other.x, y * other.y, z * other.z);
    }
    /* Multiply a vector by a scalar value */
    constexpr Vec3 operator*(const T f) const noexcept {
        return Vec3(x * f, y * f, z * f);
    }
    /* Multiply a vector by a scalar value */
    constexpr friend Vec3 operator*(const T f, const Vec3 & self) noexcept {
        return self * f;
    }
    /// Division
    /* Divide two vectors componentwise */
    constexpr Vec3 operator/(const Vec3 & other) const {
        return Vec3(x / other.x, y / other.y, z / other.z);
    }
    /* Divide a vector by a scalar value */
    constexpr Vec3 operator/(const T f) const {
        return Vec3(x / f, y / f, z / f);
    }
    /* Divide a scalar by a vector, returning a new vector */
    constexpr friend Vec3 operator/(const T f, const Vec3 & self) {
        return Vec3(f / self.x, f / self.y, f / self.z);
    }

    /// Geometric operations
    /* Dot product */
    constexpr float dot(const Vec3 & other) const noexcept {
        return x * other.x + y * other.y + z * other.z;
    }
    /* Cross product */
    constexpr Vec3 cross(const Vec3 & other) const noexcept {
        return Vec3(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
    }
    /* Squared norm of the vector */
    constexpr T squared_norm() const noexcept {
        return x * x + y * y + z * z;
    }
    /* Norm of the vector */
    inline T norm() const noexcept {
        return sqrt(squared_norm());
    }
    /* Normalize the vector */
    inline Vec3 unit_vector() const {
        return *this / norm();
    }
    /* Get the distance between two points */
    inline T distance(const Vec3 & other) const {
        return (other - *this).norm();
    }

    /// Colour functions
    /* Red component */
    constexpr uint8_t red() const noexcept {
        return static_cast<uint8_t>(utils::clamp(x) * utils::MAX_COLOUR);
    }
    /* Green component */
    constexpr uint8_t green() const noexcept {
        return static_cast<uint8_t>(utils::clamp(y) * utils::MAX_COLOUR);
    }
    /* Blue component */
    constexpr uint8_t blue() const noexcept {
        return static_cast<uint8_t>(utils::clamp(z) * utils::MAX_COLOUR);
    }

    /* Print the vector for debugging purposes */
    template <class charT, class charTraits = std::char_traits<charT>>
    inline friend std::basic_ostream<charT, charTraits> & operator<<(std::basic_ostream<charT, charTraits> & os, const Vec3 & self) {
        return os << "Vec3(" << self.x << ", " << self.y << ", " << self.z << ")";
    }

    /* Output the colour to a stream */
    template <class charT, class charTraits = std::char_traits<charT>>
    inline void write(std::basic_ostream<charT, charTraits> & os) const {
        os << red() << green() << blue();
    }
};

/* Type alias */
template <class T>
using Point3 = Vec3<T>;

}; // namespace vec3

/* RGB colours */
namespace colour {
/* RGB colour type */
using Colour = vec3::Vec3<float>;
/* For the americans */
using Color = Colour;

/* Pure red colour */
constexpr Colour RED(1, 0, 0);
/* Pure green colour */
constexpr Colour GREEN(0, 1, 0);
/* Pure blue colour */
constexpr Colour BLUE(0, 0, 1);
/* Pure yellow colour */
constexpr Colour YELLOW(1, 1, 0);
/* Pure cyan colour */
constexpr Colour CYAN(0, 1, 1);
/* Pure magenta colour */
constexpr Colour MAGENTA(1, 0, 1);
/* Pure black colour */
constexpr Colour BLACK(0, 0, 0);
/* Pure white colour */
constexpr Colour WHITE(1, 1, 1);
} // namespace colour

/* Standard vec3 and point3 implementation */
using Vec3 = vec3::Vec3<double>;
using Point3 = vec3::Point3<double>;
using colour::Colour;

/* Compare two colours for equality */
constexpr bool operator==(const Colour & a, const Colour & b) noexcept {
    return a.red() == b.red() && a.green() == b.green() && a.blue() == b.blue();
}

/* Compare two colours for inequality */
constexpr bool operator!=(const Colour & a, const Colour & b) noexcept {
    return a.red() != b.red() || a.green() != b.green() || a.blue() != b.blue();
}

#endif
