#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"
#include "game/Unk_ov006_0235f690.h"

void Ov006_SetEntryTile(int tile);
void Sound_PlaySe(int id);
void Ov006_ChooseButton38(void);
void Ov006_ChooseButton34(void);
int Ov006_GetUnitCodeAt(int x, int y);
void Ov006_LoadToolUnitPreview(void);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern Unk_ov006_0235f690* data_ov006_0236d640;

#define STATE (data_027c027c->unk_48[data_027c0284])
#define OV6 data_ov006_0236d640
#define TILE(x, y) (STATE->mapTiles->tiles[(x) + STATE->mapTiles->rowStart[y]])

/* Takes the terrain (mode 0) or unit (mode 1) under the cursor as the
 * current tool, like an eyedropper. Terrain: when the tile differs from
 * toolTile, Ov006_SetEntryTile gets the tile value and button 38 is chosen.
 * Unit: when the unit there differs from toolUnit of army unitArmy, both are
 * set from it and button 34 is chosen. An empty tile reads as
 * UNIT_TYPE_COUNT and is skipped. */
void Ov006_PickToolAtCursor(int mode) {
  int code;
  int tile;

  switch (mode) {
    case 0:
      tile = TILE(OV6->cursorX, OV6->cursorY);
      if (OV6->toolTile == tile) {
        return;
      }
      Ov006_SetEntryTile(tile);
      Sound_PlaySe(0x1000a);
      Ov006_ChooseButton38();
      break;
    case 1:
      code = ((OV6->unitArmy - 1) << 6) | (OV6->toolUnit & 63);
      if (code == Ov006_GetUnitCodeAt(OV6->cursorX, OV6->cursorY)) {
        return;
      }
      code = Ov006_GetUnitCodeAt(OV6->cursorX, OV6->cursorY);
      if ((code & 63) >= UNIT_TYPE_COUNT) {
        return;
      }
      OV6->toolUnit = code & 63;
      OV6->unitArmy = (code >> 6) + 1;
      Ov006_LoadToolUnitPreview();
      Sound_PlaySe(0x1000a);
      Ov006_ChooseButton34();
      break;
  }
}
