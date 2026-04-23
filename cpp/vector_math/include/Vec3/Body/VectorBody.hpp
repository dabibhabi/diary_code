#pragma once

/**
 * @brief 3D vector in body-fixed (vehicle) coordinates.
 *
 * Represents forces, velocities, or positions relative to a vehicle's
 * own orientation: forward along its heading, right perpendicular to it,
 * and up along the vertical. Data-only; conversions live in Vec3
 * and require the vehicle's yaw angle.
 */
struct VectorBody {
    double forward;  ///< Longitudinal component (positive = forward)
    double right;    ///< Lateral component (positive = right)
    double up;       ///< Vertical component (positive = up)

    VectorBody();                                         // Initialize to (0, 0, 0)
    VectorBody(double forward, double right, double up);  // Initialize with given components
};
