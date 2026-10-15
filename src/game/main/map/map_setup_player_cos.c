#include "types.h"
#include "game/co_data.h"
#include "game/Unk_02080440.h"
#include "game/map_header.h"
#include "game/unlocks.h"

#define MAP_CO_NONE 0 /* MapHeader.co: no CO in this slot */

extern u8 data_02167d30[][2];
extern u32 gLinkPlay;

u8 (*GetCoPicksForMode(int mode))[2];
void SetupCoSelection(void);
int Campaign_IsHardMode(void);
int func_020c3d08(int mapId);
u8 PickFreeCo(int coId);
void SetPlayer1TagPartner(void);

/* Fills the front and back CO of players 1-4 for the map being started.
 * gameMode picks the source (meaning of its values unknown): the map
 * header (cases 1 and 3; case 0 also lets the first players pick), or
 * the CO selection buffer from GetCoPicksForMode, falling back to
 * data_02167d30 when the chosen CO is not unlocked. */
void Map_SetupPlayerCos(void) {
  u8(*cos)[2];
  int player;
  int slot;
  u8 co;
  /* needed to match: the clearing loop has its own counters */
  int clearPlayer;
  int clearSlot;

  cos = GetCoPicksForMode(MAP_STATE->gameMode);
  SetupCoSelection();

  for (clearPlayer = 1; clearPlayer < ARMY_SLOT_COUNT; clearPlayer++) {
    for (clearSlot = 0; clearSlot < PLAYER_CO_COUNT; clearSlot++) {
      MAP_STATE->playerCos[clearPlayer][clearSlot] = 0;
    }
  }

  for (player = 1; player < ARMY_SLOT_COUNT; player++) {
    for (slot = 0; slot < PLAYER_CO_COUNT; slot++) {
      switch (MAP_STATE->gameMode) {
        case 0:
          if (Campaign_IsHardMode() == 1 &&
              player - 1 < func_020c3d08(MAP_STATE->unk_1c4->mapId)) {
            MAP_STATE->playerCos[player][slot] = PickFreeCo(cos[player][slot]);
            break;
          }
          /* fall through */
        case 1:
        case 3:
          co = Ov000_gMapHeaders[MAP_STATE->unk_1c4->mapId].co[player][slot];
          if (co != MAP_CO_NONE) {
            if (co != CO_PLAYER_PICKS) {
              MAP_STATE->playerCos[player][slot] = co;
            } else {
              MAP_STATE->playerCos[player][slot] =
                  PickFreeCo(cos[player][slot]);
            }
          } else {
            MAP_STATE->playerCos[player][slot] = 0;
          }
          break;
        default:
          if (gLinkPlay != 0) {
            MAP_STATE->playerCos[player][slot] =
                data_02167d30[player][slot] & 0x7f;
            break;
          }
          /* bit 7 of a selection byte is a flag (meaning unknown) */
          MAP_STATE->playerCos[player][slot] = cos[player][slot] & 0x7f;
          if (MAP_STATE->twoMaps == 1 &&
              (MAP_STATE->playerCos[player][slot] & 0x7f) == 0) {
            if (slot == 1 && data_02167d30[player][1] ==
                                 (MAP_STATE->playerCos[player][0] & 0x7f)) {
              cos[player][slot] = data_02167d30[player][0];
            } else {
              cos[player][slot] = data_02167d30[player][slot];
            }
            MAP_STATE->playerCos[player][slot] = cos[player][slot] & 0x7f;
          }
          /* a locked CO resets both slots to the defaults */
          if (!Unlock_IsSet(cos[player][slot])) {
            cos[player][0] = data_02167d30[player][0];
            cos[player][1] = data_02167d30[player][1];
            MAP_STATE->playerCos[player][0] = cos[player][0] & 0x7f;
            MAP_STATE->playerCos[player][1] = cos[player][1] & 0x7f;
          }
          break;
      }
    }
  }

  SetPlayer1TagPartner();
}
