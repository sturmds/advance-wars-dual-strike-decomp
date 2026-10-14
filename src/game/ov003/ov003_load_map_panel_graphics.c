#include "types.h"
#include "game/screen_other_map.h"

void File_UnpackTo(void* dest, const char* fileName);
static char Ov003_MapPanelMainPath[] = {
#include "data/ov003/Ov003_MapPanelMainPath.inc.c"
};
extern char Ov003_MapPanelSeaPath[];
extern char Ov003_MapPanelRiverPath[];
extern char data_ov003_023649f4[];
extern u8 data_ov003_02365d10[];

void Ov003_LoadMapPanelGraphics(void) {
  File_UnpackTo(
      data_027c0280->bgVram +
          (data_027c0280->bgCnt[data_027c0280->layer0Bg].charBase << 14),
      Ov003_MapPanelMainPath);
  File_UnpackTo((void*)0x06610000, Ov003_MapPanelSeaPath);
  File_UnpackTo((void*)0x06618000, Ov003_MapPanelRiverPath);
  File_UnpackTo(data_ov003_02365d10, data_ov003_023649f4);
}
