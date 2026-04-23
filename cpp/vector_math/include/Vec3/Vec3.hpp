#pragma once
#include "Vec3/Body/VectorBody.hpp"
#include "Vec3/Cartesian/VectorCartesian.hpp"
#include "Vec3/Cylindrical/VectorCylindrical.hpp"
#include "Vec3/Frenet/VectorFrenet.hpp"
#include "Vec3/Spherical/VectorSpherical.hpp"

/**
 * @brief Main 3D vector class supporting multiple coordinate systems.
 *
 * Stores Cartesian (x, y, z) as the canonical internal representation.
 * Provides factory methods to construct from other coordinate systems
 * (spherical, cylindrical, body-fixed, Frenet) and conversion methods
 * to extract those representations. All arithmetic operates on the
 * Cartesian form.
 */
class Vec3 {
public:
    VectorCartesian cartesian;  ///< Canonical Cartesian representation

    Vec3();                              // Initialize to origin (0, 0, 0)
    Vec3(double x, double y, double z);  // Initialize from x, y, z components
    Vec3(const VectorCartesian& cart);   // Initialize from a VectorCartesian

    // --- Factory methods: construct Vec3 from other systems ---
    static Vec3 fromSpherical(double r, double theta, double phi);  // Create from spherical components
    static Vec3 fromSpherical(const VectorSpherical& sph);          // Create from VectorSpherical struct
    static Vec3 fromCylindrical(double r, double theta, double z);  // Create from cylindrical components
    static Vec3 fromCylindrical(const VectorCylindrical& cyl);      // Create from VectorCylindrical struct
    static Vec3 fromBody(const VectorBody& body, double yaw);       // Create from body frame given yaw
    static Vec3 fromFrenet(const VectorFrenet& frenet, const Vec3& pathOrigin,
                           double pathHeading);  // Create from Frenet given path info

    // --- Convert to other coordinate systems ---
    VectorSpherical   toSpherical() const;       // Convert to spherical (r, theta, phi)
    VectorCylindrical toCylindrical() const;     // Convert to cylindrical (r, theta, z)
    VectorBody        toBody(double yaw) const;  // Convert to body frame given yaw

    // --- Arithmetic ---
    Vec3   operator+(const Vec3& other) const;  // Vector addition
    Vec3   operator-(const Vec3& other) const;  // Vector subtraction
    Vec3   operator*(double scalar) const;      // Scalar multiplication
    double dot(const Vec3& other) const;        // Dot product, returns scalar
    Vec3   cross(const Vec3& other) const;      // Cross product, returns vector
    double magnitude() const;                   // Euclidean length
    Vec3   normalized() const;                  // Unit vector in same direction
};
