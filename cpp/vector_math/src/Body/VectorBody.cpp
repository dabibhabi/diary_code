#include "Vec3/Body/VectorBody.hpp"

/**
 * @brief Default constructor. Initializes all components to zero.
 */
VectorBody::VectorBody() : forward(0), right(0), up(0) {}

/**
 * @brief Constructs a body-frame vector with specified components.
 * @param forward Longitudinal component (positive = forward).
 * @param right   Lateral component (positive = right).
 * @param up      Vertical component (positive = up).
 */
VectorBody::VectorBody(double forward, double right, double up) : forward(forward), right(right), up(up) {}
