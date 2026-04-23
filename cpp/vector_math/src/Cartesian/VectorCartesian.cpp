#include "Vec3/Cartesian/VectorCartesian.hpp"

/**
 * @brief Default constructor. Initializes all components to zero.
 */
VectorCartesian::VectorCartesian() : x(0), y(0), z(0) {}

/**
 * @brief Constructs a vector with specified components.
 * @param x The x-component.
 * @param y The y-component.
 * @param z The z-component.
 */
VectorCartesian::VectorCartesian(double x, double y, double z) : x(x), y(y), z(z) {}

/**
 * @brief Adds two vectors component-wise.
 * @param other The vector to add.
 * @return A new vector (a.x+b.x, a.y+b.y, a.z+b.z).
 */
VectorCartesian VectorCartesian::operator+(const VectorCartesian& other) const {
    return {x + other.x, y + other.y, z + other.z};
}

/**
 * @brief Subtracts another vector component-wise.
 * @param other The vector to subtract.
 * @return A new vector (a.x-b.x, a.y-b.y, a.z-b.z).
 */
VectorCartesian VectorCartesian::operator-(const VectorCartesian& other) const {
    return {x - other.x, y - other.y, z - other.z};
}

/**
 * @brief Multiplies each component by a scalar.
 * @param scalar The scaling factor.
 * @return A new scaled vector.
 */
VectorCartesian VectorCartesian::operator*(double scalar) const {
    return {x * scalar, y * scalar, z * scalar};
}

/**
 * @brief Computes the dot product (inner product) of two vectors.
 * @param other The other vector.
 * @return The scalar result: a.x*b.x + a.y*b.y + a.z*b.z.
 */
double VectorCartesian::dot(const VectorCartesian& other) const {
    return x * other.x + y * other.y + z * other.z;
}

/**
 * @brief Computes the cross product of two vectors.
 * @param other The other vector.
 * @return A new vector perpendicular to both inputs (right-hand rule).
 */
VectorCartesian VectorCartesian::cross(const VectorCartesian& other) const {
    return {y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x};
}

/**
 * @brief Computes the Euclidean norm (length) of the vector.
 * @return sqrt(x^2 + y^2 + z^2).
 */
double VectorCartesian::magnitude() const {
    return std::sqrt(x * x + y * y + z * z);
}

/**
 * @brief Returns a unit vector in the same direction.
 * @return The normalized vector. Returns (0,0,0) if magnitude is zero.
 */
VectorCartesian VectorCartesian::normalized() const {
    double mag = magnitude();
    if (mag == 0)
        return {0, 0, 0};
    return {x / mag, y / mag, z / mag};
}
