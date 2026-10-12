#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

extern Unk_02080440* data_027c027c;
extern Unk_027c0288 data_027c0288;
extern u8 data_02157668[];
extern u8 data_02157618[];

void Screen_SetBg0Mode(int mode);
void Screen_SetBgMode(int mode);
void Screen_InitLayers(const void* config);

/* Gives the current screen BG mode 0 and its layer config. On the main
 * screen BG0 shows tile maps, not the 3D engine. Every caller passes
 * data_02166dc8, which it does not read. */
void Screens_SetUp2dLayers(const void* unused) {
  if (data_027c027c == data_027c0288.unk_00) {
    Screen_SetBg0Mode(0);
    Screen_SetBgMode(0);
    Screen_InitLayers(data_02157668);
  } else {
    Screen_SetBgMode(0);
    Screen_InitLayers(data_02157618);
  }
}
