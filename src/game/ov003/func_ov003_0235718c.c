#include "types.h"

void Ov003_UpdateUnits(void);
void Ov003_DrawObjects(void);
void Ov003_DrawItems(void);
void Ov003_DrawUnitAnims(void);
void Ov003_DrawCornerIcons(void);

void func_ov003_0235718c(void) {
  Ov003_UpdateUnits();
  Ov003_DrawObjects();
  Ov003_DrawItems();
  Ov003_DrawUnitAnims();
  Ov003_DrawCornerIcons();
}
