#include "types.h"
#include "sdk/fs.h"

void Battle_LoadSideGraphics(void);
void Battle_LoadSideBackgrounds(void);
void Battle_LoadSideBackgroundPalettes(void);
void Battle_LoadSideTerrainLayers(void);
void Battle_ShowQuote(void);

char data_ov002_023600e4[] = {
#include "data/ov002/data_ov002_023600e4.inc.c"
};

void Battle_LoadScreen(void) {
  FS_ChangeDir(data_ov002_023600e4);
  Battle_LoadSideGraphics();
  Battle_LoadSideBackgrounds();
  Battle_LoadSideBackgroundPalettes();
  Battle_LoadSideTerrainLayers();
  Battle_ShowQuote();
}
