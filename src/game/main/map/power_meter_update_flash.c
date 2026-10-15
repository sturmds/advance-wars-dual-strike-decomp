#include "types.h"
#include "game/Unk_02080440.h"
#include "game/player.h"

BOOL Player_IsPowerActive(int player);
BOOL CanUseCoPower(int player, int slot);
BOOL CanUseTagPower(int player);

#define STATE_50 (data_027c027c->unk_50[data_027c0284])

#define POWER_FLASH_OFF 0
#define POWER_FLASH_ON 7

/* Runs each frame: passes every player in the game a power meter frame
 * through the state's unk_20 hook. The cycle has 16 steps of 4 frames.
 * With a power on, the meter gets the step itself; when CanUseTagPower holds
 * (tag power ready, a guess) it lights on steps 15 and 1; with a CO Power
 * ready on step 15 only; otherwise it is off. */
void PowerMeter_UpdateFlash(int frameCount) {
  u16 player;
  s16 frame;

  frame = (frameCount >> 2) & 15;
  for (player = 1; player <= PLAYER_SLOT_LAST; player++) {
    if (MAP_STATE->players[player].controller == PLAYER_CONTROLLER_NONE) {
      continue;
    }
    /* needed to match: frame is never negative, but the original tests it */
    if (Player_IsPowerActive(player) && frame >= 0) {
      if (STATE_50->unk_20 != NULL) {
        STATE_50->unk_20(player, frame);
      }
    } else if (CanUseTagPower(player)) {
      /* needed to match: case 15 first */
      switch (frame) {
        case 15:
          if (STATE_50->unk_20 != NULL) {
            STATE_50->unk_20(player, POWER_FLASH_ON);
          }
          break;
        case 0:
          if (STATE_50->unk_20 != NULL) {
            STATE_50->unk_20(player, POWER_FLASH_OFF);
          }
          break;
        case 1:
          if (STATE_50->unk_20 != NULL) {
            STATE_50->unk_20(player, POWER_FLASH_ON);
          }
          break;
        default:
          if (STATE_50->unk_20 != NULL) {
            STATE_50->unk_20(player, POWER_FLASH_OFF);
          }
          break;
      }
    } else if (CanUseCoPower(player, 0)) {
      if (frame == 15) {
        if (STATE_50->unk_20 != NULL) {
          STATE_50->unk_20(player, POWER_FLASH_ON);
        }
      } else {
        if (STATE_50->unk_20 != NULL) {
          STATE_50->unk_20(player, POWER_FLASH_OFF);
        }
      }
    } else {
      if (STATE_50->unk_20 != NULL) {
        STATE_50->unk_20(player, POWER_FLASH_OFF);
      }
    }
  }
}
