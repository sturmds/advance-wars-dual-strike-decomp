#include "types.h"
#include "game/ov003_unit.h"
#include "game/Unk_02160ac0.h"
#include "game/win_quote.h"

void Co_QueuePictureSpriteAtTile(int x, int y, int attr, int tile, int list);
void Ov003_StepFrameCycles(void);
void Ov003_UpdateRowsAndPalette(void);
void Ov003_DrawRows(void);

/* Runs three other draw routines, then queues two sprites at
 * the bottom of the screen (x 32 and x 224) when playing alone, or one
 * sprite per active row (tile 208 + 36 * row, palette row + 2) when linked. */
void Ov003_DrawHudSprites(void) {
  int i;

  Ov003_StepFrameCycles();
  Ov003_DrawRows();
  Ov003_UpdateRowsAndPalette();
  if (data_02160ac0->unk_7c == 0) {
    Sprite_QueueCoPortrait(32, 160, 0x2800, 208, 2, TRUE);
    Sprite_QueueCoPortrait(224, 160, 0x3800, 640, 2, FALSE);
    return;
  }
  for (i = 0; i < data_ov003_02364928->rowCount; i++) {
    if (data_ov003_02364928->rows[i].active) {
      Co_QueuePictureSpriteAtTile(
          (data_ov003_02364928->rows[i].unk_3b_4 * 4 + 1) * 8 + 24 & 0x1ff, 168,
          (i + 2) << 12 | 0x800, 208 + i * 36, 2);
    }
  }
}
