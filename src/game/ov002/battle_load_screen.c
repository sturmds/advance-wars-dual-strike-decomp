#include "types.h"

void FS_ChangeDir(const char* dir);
void Battle_LoadSideGraphics(void);
void Battle_LoadSideBackgrounds(void);
void Battle_LoadSideBackgroundPalettes(void);
void Battle_LoadSideTerrainLayers(void);
void Battle_ShowQuote(void);

extern const char data_ov002_023600e4[];

void Battle_LoadScreen(void) {
  FS_ChangeDir(data_ov002_023600e4);
  Battle_LoadSideGraphics();
  Battle_LoadSideBackgrounds();
  Battle_LoadSideBackgroundPalettes();
  Battle_LoadSideTerrainLayers();
  Battle_ShowQuote();
}
