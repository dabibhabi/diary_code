/* CPU-side translation unit for xorshift. Body lives in c_api/xorshift.h
 * via NM_INLINE; this TU just exposes external symbols by including the
 * header in a way that forces emission. We do that by defining a thin
 * non-inline wrapper. */

#include "c_api/xorshift.h"

uint64_t nm_xorshift64_extern(uint64_t* state) { return xorshift64_next(state); }

double nm_xorshift_to_unit_extern(uint64_t r) { return xorshift_to_unit(r); }
