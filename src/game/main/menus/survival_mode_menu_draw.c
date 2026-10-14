#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/oam_cells.h"
#include "game/survival_record.h"

void Screen_SetCurrentGfx(void* state);
void Sprite_QueueAt(int a0, int x, int y, void* a3, int attr, int tile);
void Co_QueueSmallPictureSprite(int x, int y, int attr, int a3);
int SurvivalModeMenu_GetRowMode(int row);
int SurvivalRecord_GetCo(int mode, int a1);
int SurvivalRecord_Exists(int mode);
int SurvivalMode_HasSecondRow(void);

extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;

/* Draws one frame of the Survival mode menu on the top screen: a label per
 * row (the second row only when SurvivalMode_HasSecondRow allows it) and, for each row
 * whose mode has a saved record (SurvivalRecord_Exists), the record's COs (pictures
 * row * 2 and row * 2 + 1, with palette and tiles to match) and an icon.
 * Needed to match: the first picture's attributes written from row, the
 * second's from a local. */
void SurvivalModeMenu_Draw(void) {
  Unk_027c0280* savedState = data_027c0280;
  int row;
  int mode;
  int picture;

  Screen_SetCurrentGfx(data_027c0288.unk_00);
  Sprite_QueueAt(0, 48, 38, &data_027c01c0, 0x4030, 0);
  if (SurvivalMode_HasSecondRow() == 1) {
    Sprite_QueueAt(0, 48, 102, &data_027c01c0, 0x4050, 0);
  }
  for (row = 0; row < SURVIVAL_MENU_ROW_COUNT; row++) {
    mode = SurvivalModeMenu_GetRowMode(row);
    if (SurvivalRecord_Exists(mode) != 0) {
      Co_QueueSmallPictureSprite(32, (88 + row * 64) & 0xff,
                                 (row << 13) | (row * 24), (row * 2) & 1);
      if (SurvivalRecord_GetCo(mode, 1) != 0) {
        picture = row * 2 + 1;
        Co_QueueSmallPictureSprite(64, (88 + row * 64) & 0xff,
                                   (picture * 12) | (picture << 12),
                                   picture & 1);
      }
      Sprite_QueueAt(0, 200, (72 + row * 64) & 0xff, &data_027c0040,
                     (112 + row * 4) | 0x5000, 0);
    }
  }
  Screen_SetCurrentGfx(savedState);
}
