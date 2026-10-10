#include "types.h"

void Ov003_UpdateUnits(void);
void Ov003_DrawObjects(void);
void func_ov003_0235dd48(void);
void Ov003_DrawUnitAnims(void);
void Ov003_DrawCornerIcons(void);

void func_ov003_0235718c(void)
{
    Ov003_UpdateUnits();
    Ov003_DrawObjects();
    func_ov003_0235dd48();
    Ov003_DrawUnitAnims();
    Ov003_DrawCornerIcons();
}
