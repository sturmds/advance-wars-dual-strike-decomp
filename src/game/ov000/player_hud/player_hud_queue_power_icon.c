#include "types.h"
#include "game/Unk_02080440.h"

extern u8 data_ov000_0234ec18[];
extern u8 data_ov000_0234ec28[];
extern u8 data_ov000_0234ec38[];

void Sprite_QueueAt(int list, int x, int y, void* data, int attr, int tile);

/* Queues the power icon of a player slot at (x, y): one icon while a tag
 * power is on (both COs), otherwise one by the front CO's power state. */
void PlayerHud_QueuePowerIcon(int x, int y, int slot, int palette) {
  BOOL tagPowerOn = FALSE;

  if (MAP_STATE->players[slot].back.id != 0) {
    BOOL isTagHalf = MAP_STATE->players[slot].tagPowerHalf == TAG_POWER_FRONT ||
                     MAP_STATE->players[slot].tagPowerHalf == TAG_POWER_BACK;

    if (isTagHalf) {
      tagPowerOn = TRUE;
    }
  }
  if (tagPowerOn) {
    Sprite_QueueAt(0, x - 32, y, data_ov000_0234ec28, (palette << 12) | 0x38,
                   0);
    return;
  }
  switch (MAP_STATE->players[slot].front.powerState) {
    case 1:
    default:
      Sprite_QueueAt(0, x - 8, y, data_ov000_0234ec18, (palette << 12) | 0x20,
                     0);
      break;
    case 2:
      Sprite_QueueAt(0, x - 24, y, data_ov000_0234ec38, (palette << 12) | 0x2a,
                     0);
      break;
  }
}
