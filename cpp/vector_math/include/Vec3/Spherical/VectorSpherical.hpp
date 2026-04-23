#pragma once

/**
 * @brief 3D vector in spherical coordinates (r, theta, phi).
 *
 * Represents a point using distance from origin, polar angle from the
 * +z axis (theta), and azimuthal angle from the +x axis in the
 * xy-plane (phi). Data-only; conversions live in Vec3.
 */
struct VectorSpherical {
    double r;      ///< Radius: distance from origin
    double theta;  ///< Polar angle from +z axis [0, pi]
    double phi;    ///< Azimuthal angle from +x in xy-plane (-pi, pi]

    VectorSpherical();                                    // Initialize to (0, 0, 0)
    VectorSpherical(double r, double theta, double phi);  // Initialize with given components
};
