#include "Vec3/Cylindrical/VectorCylindrical.hpp"

/**
 * @brief Default constructor. Initializes all components to zero.
 */
VectorCylindrical::VectorCylindrical() : r(0), theta(0), z(0) {}

/**
 * @brief Constructs a cylindrical vector with specified components.
 * @param r     Radial distance from the z-axis.
 * @param theta Angle from the +x axis in the xy-plane in radians.
 * @param z     Height along the z-axis.
 */
VectorCylindrical::VectorCylindrical(double r, double theta, double z) : r(r), theta(theta), z(z) {}
