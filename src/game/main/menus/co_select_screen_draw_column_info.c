#include "types.h"
#include "game/force_rank_screen.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/co_select_screen.h"
#include "game/oam_cells.h"

void Screen_SetCurrentGfx(void* state);
void Sprite_QueueAt(int list, int x, int y, void* data, int attr, int tile);
int Co_GetRank(u32 co);
int MainLoop_GetFrameCount(void);
BOOL Skills_AreDisabled(void);
void CoSelectScreen_DrawRank(int x, int y, u8 rank);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern int data_027c0284;
extern Unk_027c0288 data_027c0288;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* The CO picked for player slot column (1-4), front or back row. */
#define PICKED_CO(column, row) \
  (((u8(*)[2])data_0229c73c->coGrid)[(column) - 1][row])

/* Draws the panel of the cursor's grid column on the sub screen: a label
 * sprite per column, the rank of the front CO and (with two rows) of the
 * back CO, a picture sprite of 64 tiles per column with a second one on
 * top every other 32 frames in Campaign, and unless skills are off the
 * first 4 skill icons of each CO (tile 0x458 + 4 per skill id from 32). */
void CoSelectScreen_DrawColumnInfo(void) {
  Unk_027c0280* saved0280 = data_027c0280;
  int i;
  u8 skill;

  Screen_SetCurrentGfx(data_027c0288.unk_04);
  Sprite_QueueAt(0, 112, 136, &data_027c0060, 0x2000,
                 (data_0229c73c->cursorColumn - 1) * 8 + 864);
  if (PICKED_CO(data_0229c73c->cursorColumn, 0) != 0) {
    CoSelectScreen_DrawRank(
        40, 152, Co_GetRank(PICKED_CO(data_0229c73c->cursorColumn, 0)));
  }
  if (data_0229c73c->rowCount == 2 &&
      PICKED_CO(data_0229c73c->cursorColumn, 1) != 0) {
    CoSelectScreen_DrawRank(
        168, 152, Co_GetRank(PICKED_CO(data_0229c73c->cursorColumn, 1)));
  }
  Sprite_QueueAt(4, 96, 152, &data_027c0008, 0x4000,
                 ((data_0229c73c->cursorColumn - 1) << 6) + 920);
  if ((MainLoop_GetFrameCount() & 32) &&
      STATE->gameMode == GAME_MODE_CAMPAIGN) {
    Sprite_QueueAt(3, 96, 152, &data_027c0008, 0x4000,
                   ((data_0229c73c->cursorColumn - 1) << 6) + 952);
  }
  if (!Skills_AreDisabled()) {
    for (i = 0; i < FORCE_RANK_SLOT_COUNT; i++) {
      skill = data_0229c73c->cellSkills[data_0229c73c->cursorColumn - 1][0][i];
      if (skill == 0 || skill == 0xff) {
        break;
      }
      Sprite_QueueAt(2, (32 + i * 16) & 0x1ff, 168, &data_027c0040, 0x5000,
                     0x458 + (skill - 32) * 4);
    }
    for (i = 0; i < FORCE_RANK_SLOT_COUNT; i++) {
      skill = data_0229c73c->cellSkills[data_0229c73c->cursorColumn - 1][1][i];
      if (skill == 0 || skill == 0xff) {
        break;
      }
      Sprite_QueueAt(2, (160 + i * 16) & 0x1ff, 168, &data_027c0040, 0x5000,
                     0x458 + (skill - 32) * 4);
    }
  }
  Screen_SetCurrentGfx(saved0280);
}
