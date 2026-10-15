#include "types.h"
#include "game/Unk_ov006_0235f690.h"
#include "game/unlocks.h"

void Ov006_LoadToolUnitPreview(void);
void Ov006_LoadEntryTerrainGraphics(void);
void func_ov006_02357734(void);

extern s8 data_ov006_0236a7b4[];
extern s8 data_ov006_0236a7d0[];

#define STATE data_ov006_0236d640

/* Updates the view of entry `index`, depending on STATE->unitMode. With 1: toolUnit
 * from a table picked by game flag 0x2b, redrawn by Ov006_LoadToolUnitPreview when it
 * or unitArmy changed. With 0: toolTile from data_ov006_0236a7a0, redrawn by
 * Ov006_LoadEntryTerrainGraphics when it changed; for ids 6, 8, 10, 11, 14
 * and 22 terrainPage goes in bits 5 and up. Both set bit 3 of the flags. */
void Ov006_UpdateEntryView(int index) {
  int a;
  int shift;

  if (STATE->unitMode == 1) {
    if (GameFlag_IsSet(0x2b) == 1) {
      a = data_ov006_0236a7d0[index];
    } else {
      a = data_ov006_0236a7b4[index];
    }
    if (STATE->toolUnit != a || STATE->unitArmy != STATE->unitPage) {
      STATE->toolUnit = a;
      STATE->unk_1e = index;
      STATE->unitArmy = STATE->unitPage;
      Ov006_LoadToolUnitPreview();
    }
    func_ov006_02357734();
    STATE->unk_00 |= 8;
  }
  if (STATE->unitMode != 0) {
    return;
  }
  shift = STATE->terrainPage;
  a = data_ov006_0236a7a0.ids[index];
  if (shift == 0 && a == 8) {
    return;
  }
  switch (a) {
    case 6:
    case 8:
    case 10:
    case 11:
    case 14:
    case 22:
      a = (a & 31) | (shift << 5);
      break;
  }
  if (STATE->toolTile != a) {
    STATE->toolTile = a;
    STATE->unk_20 = index;
    STATE->tileArmy = STATE->terrainPage;
    Ov006_LoadEntryTerrainGraphics();
  }
  func_ov006_02357734();
  STATE->unk_00 |= 8;
}
