#include "types.h"
#include "game/force_rank_screen.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/text_print.h"

void Bg_ClearRect(u16* map, u16 x, u16 y, int width, u16 height);
void ForceRankScreen_PutNoSkillIcon(int x, int y);
void ForceRankScreen_PutSkillIcon(int x, int y, int skill);
void Gfx_RequestLayerCopy(int layer);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;

/* Redraws one row of the skill slot list: clears 30x2 tiles at (x, y), then
 * puts the skill's icon, name and description (name at x + 2, description at
 * x + 11; textTile is the first text tile, the description uses the 16
 * after it). Skill 0 draws the "no skill" entry, on the first screen only.
 * Needed to match: the layer copy is written once in each branch. */
void ForceRankScreen_DrawSlotRow(int x, int y, int textTile, int skill) {
  Bg_ClearRect(data_027c0280->layer0Map, x, y, 30, 2);
  if (skill == 0) {
    if ((void*)data_027c027c == data_027c0288.unk_00) {
      ForceRankScreen_PutNoSkillIcon(x, y);
      Text_PrintId(x + 2, y, data_027c0280->layer0Map, 0x7b0000c0, 0, textTile);
    }
    Gfx_RequestLayerCopy(0);
    return;
  }
  ForceRankScreen_PutSkillIcon(x, y, skill);
  Text_PrintId(x + 2, y, data_027c0280->layer0Map,
               Ov000_gForceRankTable[skill].name.id, 0, textTile);
  Text_PrintId(x + 11, y, data_027c0280->layer0Map,
               Ov000_gForceRankTable[skill].description.id, 0, textTile + 16);
  Gfx_RequestLayerCopy(0);
}
