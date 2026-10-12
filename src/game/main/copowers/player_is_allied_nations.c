#include "types.h"
#include "game/player.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/map_header.h"
#include "game/co_data.h"

void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
int Co_GetNation(int co);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern int data_027c0284;
extern Unk_027c0288 data_027c0288;

#define GAME (data_027c027c->unk_48[data_027c0284])

/* The game state of the current map, one of the two in data_027c0288. */
#define CURRENT_MAP ((&data_027c0288.unk_00)[data_027c0284])

#define SWITCH_TO(state)       \
  saved0280 = data_027c0280;   \
  Screen_SetCurrentGfx(state); \
  saved027c = data_027c027c;   \
  Screen_SetCurrent(state)

#define SWITCH_BACK()           \
  Screen_SetCurrent(saved027c); \
  Screen_SetCurrentGfx(saved0280)

/* TRUE when the player plays for the Allied Nations, which picks the
 * normal power themes over the Black Hole ones. When gameMode is not 0 the
 * front CO's nation decides. In gameMode 0 (the campaign, going by the
 * mission ids) every human player counts, and on three missions every
 * player does. */
BOOL Player_IsAlliedNations(int player) {
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;
  u16 mapId;
  BOOL human;

  if (GAME->gameMode == GAME_MODE_CAMPAIGN) {
    SWITCH_TO(CURRENT_MAP);
    mapId = GAME->unk_1c4->mapId;
    SWITCH_BACK();
    if (mapId == MAP_JAKES_TRIAL || mapId == MAP_LIGHTNING_STRIKES ||
        mapId == MAP_VERDANT_HILLS) {
      return TRUE;
    }

    SWITCH_TO(CURRENT_MAP);
    human = GAME->players[player].controller == PLAYER_CONTROLLER_HUMAN;
    SWITCH_BACK();
    return human == TRUE;
  }
  return Co_GetNation(GAME->players[player].front.id & PLAYER_CO_ID_MASK) !=
         NATION_BLACK_HOLE;
}
