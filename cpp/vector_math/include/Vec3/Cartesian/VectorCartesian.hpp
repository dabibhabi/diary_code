#pragma once
#include <cmath>

/**
 * @brief 3D vector in Cartesian coordinates (x, y, z).
 *
 * Provides standard vector arithmetic: addition, subtraction,
 * scalar multiplication, dot product, cross product, and normalization.
 * This is the canonical representation used internally by Vec3.
 */
class VectorCartesian {
public:
    double x, y, z;

    VectorCartesian();                              // Initialize to (0, 0, 0)
    VectorCartesian(double x, double y, double z);  // Initialize with given components

    VectorCartesian operator+(const VectorCartesian& other) const;  // Component-wise addition
    VectorCartesian operator-(const VectorCartesian& other) const;  // Component-wise subtraction
    VectorCartesian operator*(double scalar) const;                 // Scale each component

    double          dot(const VectorCartesian& other) const;    // Dot product, returns scalar
    VectorCartesian cross(const VectorCartesian& other) const;  // Cross product, returns vector
    double          magnitude() const;                          // Euclidean length of the vector
    VectorCartesian normalized() const;                         // Unit vector in same direction
};
