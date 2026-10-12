#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

extern Unk_02080440* data_027c027c;
extern Unk_027c0288 data_027c0288;
extern u8 data_021575c8[];
extern u8 data_02157618[];

void Screen_SetBg0Mode(int mode);
void Screen_SetBgMode(int mode);
void Screen_InitLayers(const void* config);

/* Like Screens_SetUp2dLayers, but BG0 of the main screen shows the 3D engine. */
void Screens_SetUp3dLayers(const void* unused) {
  if (data_027c027c == data_027c0288.unk_00) {
    Screen_SetBg0Mode(1);
    Screen_SetBgMode(0);
    Screen_InitLayers(data_021575c8);
  } else {
    Screen_SetBgMode(0);
    Screen_InitLayers(data_02157618);
  }
}
