#include "Vec3/Vec3.hpp"
#include <cmath>

// --- Constructors ---

/**
 * @brief Default constructor. Initializes to the origin (0, 0, 0).
 */
Vec3::Vec3() : cartesian() {}

/**
 * @brief Constructs a Vec3 from Cartesian components.
 * @param x The x-component.
 * @param y The y-component.
 * @param z The z-component.
 */
Vec3::Vec3(double x, double y, double z) : cartesian(x, y, z) {}

/**
 * @brief Constructs a Vec3 from an existing VectorCartesian.
 * @param cart The Cartesian vector to copy.
 */
Vec3::Vec3(const VectorCartesian& cart) : cartesian(cart) {}

// --- Factory: Spherical ---

/**
 * @brief Creates a Vec3 from spherical coordinates.
 *
 * Converts using:
 *   x = r * sin(theta) * cos(phi)
 *   y = r * sin(theta) * sin(phi)
 *   z = r * cos(theta)
 *
 * @param r     Radius (distance from origin).
 * @param theta Polar angle from +z axis in radians.
 * @param phi   Azimuthal angle from +x axis in radians.
 * @return A Vec3 with the equivalent Cartesian coordinates.
 */
Vec3 Vec3::fromSpherical(double r, double theta, double phi) { return {{r * std::sin(theta) * std::cos(phi), r * std::sin(theta) * std::sin(phi), r * std::cos(theta)}}; }  // Create from spherical components

/**
 * @brief Creates a Vec3 from a VectorSpherical struct.
 * @param sph The spherical vector.
 * @return A Vec3 with the equivalent Cartesian coordinates.
 */
Vec3 Vec3::fromSpherical(const VectorSpherical& sph) { return fromSpherical(sph.r, sph.theta, sph.phi); }  // Create from VectorSpherical struct

// --- Factory: Cylindrical ---

/**
 * @brief Creates a Vec3 from cylindrical coordinates.
 *
 * Converts using:
 *   x = r * cos(theta)
 *   y = r * sin(theta)
 *   z = z
 *
 * @param r     Radial distance from the z-axis.
 * @param theta Angle from +x axis in the xy-plane in radians.
 * @param z     Height along the z-axis.
 * @return A Vec3 with the equivalent Cartesian coordinates.
 */
Vec3 Vec3::fromCylindrical(double r, double theta, double z) { return {{r * std::cos(theta), r * std::sin(theta), z}}; }  // Create from cylindrical components

/**
 * @brief Creates a Vec3 from a VectorCylindrical struct.
 * @param cyl The cylindrical vector.
 * @return A Vec3 with the equivalent Cartesian coordinates.
 */
Vec3 Vec3::fromCylindrical(const VectorCylindrical& cyl) { return fromCylindrical(cyl.r, cyl.theta, cyl.z); }  // Create from VectorCylindrical struct

// --- Factory: Body ---

/**
 * @brief Creates a Vec3 from body-fixed coordinates and a yaw angle.
 *
 * Applies a 2D rotation by yaw to transform from the vehicle's local
 * frame (forward, right, up) into the world frame (x, y, z):
 *   x = forward * cos(yaw) - right * sin(yaw)
 *   y = forward * sin(yaw) + right * cos(yaw)
 *   z = up
 *
 * @param body The body-frame vector (forward, right, up).
 * @param yaw  The vehicle's heading angle in radians (from +x toward +y).
 * @return A Vec3 in world-frame Cartesian coordinates.
 */
Vec3 Vec3::fromBody(const VectorBody& body, double yaw) {
    double x = body.forward * std::cos(yaw) - body.right * std::sin(yaw);
    double y = body.forward * std::sin(yaw) + body.right * std::cos(yaw);
    double z = body.up;
    return {{x, y, z}};
}

// --- Factory: Frenet ---

/**
 * @brief Creates a Vec3 from Frenet coordinates given a path reference.
 *
 * Uses a linearized (straight-path) approximation:
 *   x = x0 + s * cos(heading) - d * sin(heading)
 *   y = y0 + s * sin(heading) + d * cos(heading)
 *   z = z0
 *
 * Valid when path curvature * lateral offset is small (|kappa * d| << 1).
 *
 * @param frenet      The Frenet vector (s, d, theta).
 * @param pathOrigin  The world-frame origin of the path segment.
 * @param pathHeading The heading of the path at the origin in radians.
 * @return A Vec3 in world-frame Cartesian coordinates.
 */
Vec3 Vec3::fromFrenet(const VectorFrenet& frenet, const Vec3& pathOrigin, double pathHeading) {
    double x = pathOrigin.cartesian.x + frenet.s * std::cos(pathHeading) - frenet.d * std::sin(pathHeading);
    double y = pathOrigin.cartesian.y + frenet.s * std::sin(pathHeading) + frenet.d * std::cos(pathHeading);
    double z = pathOrigin.cartesian.z;
    return {{x, y, z}};
}

// --- Conversions ---

/**
 * @brief Converts this vector to spherical coordinates.
 *
 * Computes:
 *   r     = magnitude
 *   theta = acos(z / r)
 *   phi   = atan2(y, x)
 *
 * @return A VectorSpherical with the equivalent (r, theta, phi).
 */
VectorSpherical Vec3::toSpherical() const {
    double r     = magnitude();
    double theta = (r == 0) ? 0 : std::acos(cartesian.z / r);
    double phi   = std::atan2(cartesian.y, cartesian.x);
    return {r, theta, phi};
}

/**
 * @brief Converts this vector to cylindrical coordinates.
 *
 * Computes:
 *   r     = sqrt(x^2 + y^2)
 *   theta = atan2(y, x)
 *   z     = z
 *
 * @return A VectorCylindrical with the equivalent (r, theta, z).
 */
VectorCylindrical Vec3::toCylindrical() const {
    double r     = std::sqrt(cartesian.x * cartesian.x + cartesian.y * cartesian.y);
    double theta = std::atan2(cartesian.y, cartesian.x);
    return {r, theta, cartesian.z};
}

/**
 * @brief Converts this vector to body-fixed coordinates given a yaw angle.
 *
 * Applies the inverse (transposed) rotation matrix:
 *   forward = x * cos(yaw) + y * sin(yaw)
 *   right   = -x * sin(yaw) + y * cos(yaw)
 *   up      = z
 *
 * @param yaw The vehicle's heading angle in radians.
 * @return A VectorBody with the equivalent (forward, right, up).
 */
VectorBody Vec3::toBody(double yaw) const {
    double forward = cartesian.x * std::cos(yaw) + cartesian.y * std::sin(yaw);
    double right   = -cartesian.x * std::sin(yaw) + cartesian.y * std::cos(yaw);
    return {forward, right, cartesian.z};
}

// --- Arithmetic (delegates to VectorCartesian) ---

/**
 * @brief Adds two Vec3 vectors component-wise.
 * @param other The vector to add.
 * @return A new Vec3 with summed components.
 */
Vec3 Vec3::operator+(const Vec3& other) const { return {cartesian + other.cartesian}; }  // Vector addition

/**
 * @brief Subtracts another Vec3 component-wise.
 * @param other The vector to subtract.
 * @return A new Vec3 with the difference.
 */
Vec3 Vec3::operator-(const Vec3& other) const { return {cartesian - other.cartesian}; }  // Vector subtraction

/**
 * @brief Multiplies this vector by a scalar.
 * @param scalar The scaling factor.
 * @return A new scaled Vec3.
 */
Vec3 Vec3::operator*(double scalar) const { return {cartesian * scalar}; }  // Scalar multiplication

/**
 * @brief Computes the dot product with another Vec3.
 * @param other The other vector.
 * @return The scalar dot product.
 */
double Vec3::dot(const Vec3& other) const { return cartesian.dot(other.cartesian); }  // Dot product, returns scalar

/**
 * @brief Computes the cross product with another Vec3.
 * @param other The other vector.
 * @return A new Vec3 perpendicular to both inputs.
 */
Vec3 Vec3::cross(const Vec3& other) const { return {cartesian.cross(other.cartesian)}; }  // Cross product, returns vector

/**
 * @brief Returns the Euclidean length of this vector.
 * @return The magnitude sqrt(x^2 + y^2 + z^2).
 */
double Vec3::magnitude() const { return cartesian.magnitude(); }  // Euclidean length of the vector

/**
 * @brief Returns a unit vector in the same direction.
 * @return A normalized Vec3. Returns (0,0,0) if magnitude is zero.
 */
Vec3 Vec3::normalized() const { return {cartesian.normalized()}; }  // Unit vector in same direction
