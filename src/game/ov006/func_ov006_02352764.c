#include "types.h"
#include "game/Unk_ov006_0235f690.h"

void Ov006_LoadToolUnitPreview(void);
void Ov006_LoadEntryTerrainGraphics(void);

extern Unk_ov006_0235f690* data_ov006_0236d640;

void func_ov006_02352764(void) {
  data_ov006_0236d640->toolTile = 1;
  data_ov006_0236d640->tileArmy = 1;
  data_ov006_0236d640->toolUnit = 1;
  data_ov006_0236d640->unitArmy = 1;
  Ov006_LoadToolUnitPreview();
  Ov006_LoadEntryTerrainGraphics();
}
