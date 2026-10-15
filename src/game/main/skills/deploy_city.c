#include "types.h"
#include "game/Unk_02080440.h"
#include "game/co_data.h"
#include "game/player.h"
#include "game/skills.h"
#include "game/terrain.h"

extern s8 Ov000_gGroundDeployTerrain
    []; /* nonzero where ground units can be built, by terrain id */

/* Needed to match: after the first skill check the original reloads the
 * player through the globals instead of reusing state. */
#define PLAYER(i) (data_027c027c->unk_48[data_027c0284]->players[i])

/* Whether the acting player can build ground units on this map tile.
 * Besides the usual terrain, an owned city counts while SKILL_DEPLOY_CITY
 * is set (Hachi's Super CO Power), or during any Super CO Power with Soul
 * of Hachi equipped. */
BOOL DeployCity(u8 tile) {
  u8 terrain = MAP_TILE_TERRAIN(tile);
  u8 owner = tile & 0xe0;
  Unk_02080440_48* state = data_027c027c->unk_48[data_027c0284];
  u16 playerIndex = state->currentPlayer;

  if (owner != state->unk_04) {
    return FALSE;
  }
  if (terrain == TERRAIN_CITY) {
    if (CheckSkillsEquipped(state->players[playerIndex].skills[0],
                            SKILL_DEPLOY_CITY) ||
        (PLAYER(playerIndex).front.powerState == POWER_SUPER &&
         CheckSkillsEquipped(PLAYER(playerIndex).skills[0],
                             SKILL_RANK_SOUL_OF_HACHI))) {
      return TRUE;
    }
  } else if (Ov000_gGroundDeployTerrain[MAP_TILE_TERRAIN(terrain)] !=
             0) { /* needed to match: masks twice */
    return TRUE;
  }
  return FALSE;
}
