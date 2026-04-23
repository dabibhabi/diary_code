#pragma once

/**
 * @brief 3D vector in Frenet-Serret (path-relative) coordinates.
 *
 * Describes a position relative to a reference path (e.g., road centerline)
 * using arc length, lateral offset, and heading deviation.
 * Data-only; conversions live in Vec3 and require path context.
 */
struct VectorFrenet {
    double s;      ///< Arc length along the path centerline
    double d;      ///< Lateral offset from centerline (positive = left)
    double theta;  ///< Heading relative to path tangent

    VectorFrenet();                                  // Initialize to (0, 0, 0)
    VectorFrenet(double s, double d, double theta);  // Initialize with given components
};
