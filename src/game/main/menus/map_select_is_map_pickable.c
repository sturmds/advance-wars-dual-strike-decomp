#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_header.h"

u8 Map_GetPlayerCount(u16 mapId);
int Map_Exists(u16 mapId);
u8 Map_IsFlagSet(u32 mapId);
int Map_NeedsUnlockFlag(int mapId);
u32 Link_IsActive(void);
int func_0207a1f0(void);

/* Whether map mapId goes into the map select list: a two-map battle
 * (outside the War Room) takes only 2-player maps; offline, only the first
 * three design maps (this player's own) count; in link play the map needs
 * a slot for every linked player; maps 41-44 also need their flag in
 * gMapFlags. */
BOOL MapSelect_IsMapPickable(int mapId) {
  Unk_02080440_48* state = data_027c027c->unk_48[data_027c0284];

  if (state->gameMode != GAME_MODE_WAR_ROOM && state->twoMaps == 1) {
    if (Map_GetPlayerCount(mapId) != 2) {
      return FALSE;
    }
  }
  if (Link_IsActive() == 0) {
    if (mapId >= MAP_DESIGN_FIRST && mapId < MAP_DESIGN_END &&
        mapId >= MAP_DESIGN_FIRST + 3 /* rows another player sent */) {
      return FALSE;
    }
  } else {
    if (Map_GetPlayerCount(mapId) < func_0207a1f0() + 1) {
      return FALSE;
    }
  }
  if (!Map_Exists(mapId)) {
    return FALSE;
  }
  if (Map_NeedsUnlockFlag(mapId) == 1 && !Map_IsFlagSet(mapId)) {
    return FALSE;
  }
  return TRUE;
}
