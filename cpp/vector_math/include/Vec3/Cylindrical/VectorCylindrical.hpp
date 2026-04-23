#pragma once

/**
 * @brief 3D vector in cylindrical coordinates (r, theta, z).
 *
 * Represents a point using radial distance from the z-axis,
 * angle from the +x axis in the xy-plane, and height.
 * Data-only; conversions live in Vec3.
 */
struct VectorCylindrical {
    double r;      ///< Radial distance from the z-axis
    double theta;  ///< Angle from +x axis in xy-plane
    double z;      ///< Height along z-axis

    VectorCylindrical();                                  // Initialize to (0, 0, 0)
    VectorCylindrical(double r, double theta, double z);  // Initialize with given components
};
