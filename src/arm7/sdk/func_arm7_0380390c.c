#include "types.h"

void SndHw_Disable(void);
void func_arm7_03805878(void);
void func_arm7_037fdc10(void);

void func_arm7_0380390c(void)
{
    SndHw_Disable();
    func_arm7_03805878();
    func_arm7_037fdc10();
}
