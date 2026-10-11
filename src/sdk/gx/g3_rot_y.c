#include "types.h"
#include "sdk/gx.h"

void G3_RotY(int sinVal, int cosVal)
{
    *IO_GEOM_APPLY_MATRIX_3X3 = cosVal;
    *IO_GEOM_APPLY_MATRIX_3X3 = 0;
    *IO_GEOM_APPLY_MATRIX_3X3 = -sinVal;
    *IO_GEOM_APPLY_MATRIX_3X3 = 0;
    *IO_GEOM_APPLY_MATRIX_3X3 = 0x1000;
    *IO_GEOM_APPLY_MATRIX_3X3 = 0;
    *IO_GEOM_APPLY_MATRIX_3X3 = sinVal;
    *IO_GEOM_APPLY_MATRIX_3X3 = 0;
    *IO_GEOM_APPLY_MATRIX_3X3 = cosVal;
}
