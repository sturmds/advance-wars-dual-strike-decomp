#include "types.h"
#include "game/Unk_02080440.h"

extern u8 data_ov000_0234ec00[];
extern u8 data_ov000_0234ec08[];
extern u8 data_ov000_0234ec10[];
extern char data_ov000_0234ec6c[];

void Sprite_QueueAt(int list, int x, int y, void* data, int attr, int tile);
void Oam_PutTextString(u16 x, u16 y, void* data);
void Oam_PutNumber(int x, int y, s32 value, int attr);
void CoPanel_Draw(int x, int y, int slot, int back, int coId, int powerState,
                  int fromOtherMap, int tile, int paletteSlot, int paletteBank,
                  int a6);

/* Draws one player's HUD slot at (x, y): the frame, the funds (or the hidden
 * funds mark when fog or rain hides them), and the front and back CO
 * pictures. The caller sets backFromOtherMap and backFlag together when the
 * back CO shown is a CO of the other map (two-map battle). */
void PlayerHud_DrawSlot(int x, int y, int slot, int frontCo, int backCo,
                        int frontState, int backState, int frontFromOtherMap,
                        int backFromOtherMap, int backFlag) {
  int paletteBank = slot + 10;
  int frontPalette = (slot - 1) * 2 + 3;
  int backPalette = (slot - 1) * 2 + 4;
  int frontTile = (slot - 1) * 16 + 72;
  int backTile = (slot - 1) * 16 + 80;

  Sprite_QueueAt(5, x + 4, y + 14, data_ov000_0234ec10,
                 (paletteBank << 12) | 20, 0);
  if ((MAP_STATE->unk_1c4->fog != 0 ||
       MAP_STATE->unk_1c4->weather == WEATHER_RAIN) &&
      (MAP_STATE->players[slot].visionGrids & 2) == 0) {
    Oam_PutTextString((x + 56) & 0x1ff, y + 14, data_ov000_0234ec6c);
  } else {
    Oam_PutNumber((x + 56) & 0x1ff, y + 14, MAP_STATE->players[slot].funds,
                  (paletteBank << 12) | 22);
  }
  if (frontCo != 0 && backCo != 0) {
    Sprite_QueueAt(5, x, y, data_ov000_0234ec08, (paletteBank << 12) | 0x4a8,
                   0);
  } else {
    if (frontCo != 0) {
      Sprite_QueueAt(5, x, y, data_ov000_0234ec00, (paletteBank << 12) | 0x488,
                     0);
    }
    if (backCo != 0) {
      Sprite_QueueAt(5, x, y + 20, data_ov000_0234ec00,
                     (paletteBank << 12) | 0x488, 0);
    }
  }
  if (frontCo != 0) {
    CoPanel_Draw(x, y, slot, 0, frontCo, frontState, frontFromOtherMap,
                 frontTile, frontPalette, paletteBank, 0);
  }
  if (backCo != 0) {
    CoPanel_Draw(x, y + 20, slot, 1, backCo, backState, backFromOtherMap,
                 backTile, backPalette, paletteBank, backFlag);
  }
}
