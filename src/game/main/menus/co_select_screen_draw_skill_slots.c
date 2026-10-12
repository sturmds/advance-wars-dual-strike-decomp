#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/co_select_screen.h"

void Screen_SetCurrentGfx(void* state);
BOOL Skills_AreDisabled(void);
int Co_GetRank(u32 co);
void Bg_PutSkillSlot(u16 x, u16 y, u16* map, int palette);
void Bg_PutTile5Quad(u16 x, u16 y, u16* map, int palette);
void Gfx_RequestLayerCopy(int layer);

extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;

/* CO in row `row` of the cursor column. Needed to match: coGrid read as rows of 2. */
#define COLUMN_CO(row) \
  (((u8(*)[2])data_0229c73c->coGrid)[data_0229c73c->cursorColumn - 1][row])

/* Draws the skill slots of the two COs in the cursor column on layer 1 of
 * screen data_027c0288.unk_04, row 21: one slot per rank (at most 4) from
 * column 4 for the row 0 CO and from column 20 for the row 1 CO, blank
 * tiles for the rest. No slots while skills are off or the cell is empty;
 * none for row 1 when the grid has one row. */
void CoSelectScreen_DrawSkillSlots(void) {
  int i;
  Unk_027c0280* saved;
  int slots;

  saved = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);

  if (Skills_AreDisabled() == TRUE || COLUMN_CO(0) == 0) {
    slots = 0;
  } else {
    slots = Co_GetRank(COLUMN_CO(0));
    if (slots > 4) {
      slots = 4;
    }
  }
  for (i = 0; i < slots; i++) {
    Bg_PutSkillSlot(4 + i * 2, 21, data_027c0280->layer1Map, 0);
  }
  for (; i < 4; i++) {
    Bg_PutTile5Quad(4 + i * 2, 21, data_027c0280->layer1Map, 0);
  }

  if (Skills_AreDisabled() == TRUE || data_0229c73c->rowCount == 1 ||
      COLUMN_CO(1) == 0) {
    slots = 0;
  } else {
    slots = Co_GetRank(COLUMN_CO(1));
    if (slots > 4) {
      slots = 4;
    }
  }
  for (i = 0; i < slots; i++) {
    Bg_PutSkillSlot(20 + i * 2, 21, data_027c0280->layer1Map, 0);
  }
  for (; i < 4; i++) {
    Bg_PutTile5Quad(20 + i * 2, 21, data_027c0280->layer1Map, 0);
  }

  Gfx_RequestLayerCopy(1);
  Screen_SetCurrentGfx(saved);
}
