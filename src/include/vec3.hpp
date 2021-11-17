#ifndef VEC3_HPP
#define VEC3_HPP

#include <iostream>
#include <ostream>

/* Three dimensional vectors */
namespace vector {
/* A three dimensional vector class */
template <class T>
class Vec3 {
    static_assert(std::is_floating_point<T>::value, "T must be a floating-point type!");

public:
    /* Vector components */
    T x, y, z;

    /// Constants
    /* Null vector */
    constexpr static Vec3 ZEROS() { return Vec3(0, 0, 0); }
    /* Vector of ones */
    constexpr static Vec3 ONES() { return Vec3(1, 1, 1); }
    /* Unit x vector */
    constexpr static Vec3 X() { return Vec3(1, 0, 0); }
    /* Unit y vector */
    constexpr static Vec3 Y() { return Vec3(0, 1, 0); }
    /* Unit z vector */
    constexpr static Vec3 Z() { return Vec3(0, 0, 1); }

    /// Constructors
    /* Default Constructor */
    Vec3() : x(0), y(0), z(0) {}
    /* Two dimensional constructor */
    Vec3(T x, T y) : x(x), y(y), z(0) {}
    /* Three dimensional constructor */
    Vec3(T x, T y, T z) : x(x), y(y), z(z) {}

    /// Operator overloading
    /* Get the opposite of a vector */
    inline Vec3 operator-() const {
        return Vec3(-x, -y, -z);
    }
    /* Unary + operator */
    inline Vec3 operator+() const {
        return *this;
    }
    /// Addition
    /* Add two vectors componentwise */
    inline Vec3 operator+(const Vec3 & other) const {
        return Vec3(x + other.x, y + other.y, z + other.z);
    }
    /* Add a scalar value to all components of a vector */
    inline Vec3 operator+(const T f) const {
        return Vec3(x + f, y + f, z + f);
    }
    /* Add a scalar value to all components of a vector */
    inline friend Vec3 operator+(const T f, const Vec3 & self) {
        return self + f;
    }
    /// Subtraction
    /* Substract two vectors componentwise */
    inline Vec3 operator-(const Vec3 & other) const {
        return Vec3(x - other.x, y - other.y, z - other.z);
    }
    /* Substract a scalar value from all components of a vector */
    inline Vec3 operator-(const T f) const {
        return Vec3(x - f, y - f, z - f);
    }
    /* Substract a vector from a scalar value, returning a new vector */
    inline friend Vec3 operator-(const T f, const Vec3 & self) {
        return Vec3(f - self.x, f - self.y, f - self.z);
    }
    /// Multiplication
    /* Multiply two vectors componentwise */
    inline Vec3 operator*(const Vec3 & other) const {
        return Vec3(x * other.x, y * other.y, z * other.z);
    }
    /* Multiply a vector by a scalar value */
    inline Vec3 operator*(const T f) const {
        return Vec3(x * f, y * f, z * f);
    }
    /* Multiply a vector by a scalar value */
    inline friend Vec3 operator*(const T f, const Vec3 & self) {
        return self * f;
    }
    /// Division
    /* Divide two vectors componentwise */
    inline Vec3 operator/(const Vec3 & other) const {
        return Vec3(x / other.x, y / other.y, z / other.z);
    }
    /* Divide a vector by a scalar value */
    inline Vec3 operator/(const T f) const {
        return Vec3(x / f, y / f, z / f);
    }
    /* Divide a scalar by a vector, returning a new vector */
    inline friend Vec3 operator/(const T f, const Vec3 & self) {
        return Vec3(f / self.x, f / self.y, f / self.z);
    }

    /// Geometric operations
    /* Dot product */
    inline float dot(const Vec3 & other) const {
        return x * other.x + y * other.y + z * other.z;
    }
    /* Cross product */
    inline Vec3 cross(const Vec3 & other) const {
        return Vec3(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
    }
    /* Squared norm of the vector */
    inline T squared_norm() const {
        return x * x + y * y + z * z;
    }
    /* Norm of the vector */
    inline T norm() const {
        return sqrt(squared_norm());
    }
    /* Normalize the vector */
    Vec3 unit_vector() const {
        return *this / norm();
    }
    /* Get the distance between two points */
    T distance(const Vec3 & other) const {
        return (other - *this).norm();
    }

#define _MAX_COLOUR 255.999f
    /// Colour functions
    /* Red component */
    uint8_t red() const {
        return static_cast<uint8_t>(x * _MAX_COLOUR);
    }
    /* Green component */
    uint8_t green() const {
        return static_cast<uint8_t>(y * _MAX_COLOUR);
    }
    /* Blue component */
    uint8_t blue() const {
        return static_cast<uint8_t>(z * _MAX_COLOUR);
    }
#undef _MAX_COLOUR

    /* Output the colour to a stream */
    template <class charT, class charTraits = std::char_traits<charT>>
    friend std::basic_ostream<charT, charTraits> & operator<<(std::basic_ostream<charT, charTraits> & os, const Vec3 & self) {
        return os << self.red() << self.green() << self.blue();
    }

    /* Print the vector for debugging purposes */
    void debug() {
        std::cout << "Vec3(" << x << ", " << y << ", " << z << ")";
    }
};

/* Type alias */
template <class T>
using Point3 = Vec3<T>;

}; // namespace vector

/* RGB colours */
namespace colour {
/* RGB colour type */
using Colour = vector::Vec3<float>;
/* For the americans */
using Color = Colour;

/* Pure red colour */
const Colour RED(1, 0, 0);
/* Pure green colour */
const Colour GREEN(0, 1, 0);
/* Pure blue colour */
const Colour BLUE(0, 0, 1);
/* Pure yellow colour */
const Colour YELLOW(1, 1, 0);
/* Pure cyan colour */
const Colour CYAN(0, 1, 1);
/* Pure magenta colour */
const Colour MAGENTA(1, 0, 1);
/* Pure black colour */
const Colour BLACK(0, 0, 0);
/* Pure white colour */
const Colour WHITE(1, 1, 1);
} // namespace colour

/* Standard vec3 and point3 implementation */
using vec3 = vector::Vec3<double>;
using point3 = vector::Point3<double>;

#endif
