#include "types.h"
#include "sdk/gx.h"

void func_0210bb9c(int a, int b);

extern s16 data_0214f4c4[2];

/* Sets the 3D matrix mode to 2 (position and vector together), moves by (0, 0, 0xf00)
 * and (0, -0x30, 0) with MTX_TRANS, then multiplies a rotation made from the
 * two values at data_0214f4c4 (func_0210bb9c is MTX_MULT_3x3). */
void Battle_SetupCamera(void)
{
    *REG_MTX_MODE = 2;
    *REG_MTX_TRANS = 0;
    *REG_MTX_TRANS = 0;
    *REG_MTX_TRANS = 0xf00;
    *REG_MTX_TRANS = 0;
    *REG_MTX_TRANS = -0x30;
    *REG_MTX_TRANS = 0;
    func_0210bb9c(data_0214f4c4[0], data_0214f4c4[1]);
}
