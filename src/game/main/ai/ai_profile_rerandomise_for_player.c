#include "types.h"
#include "game/ai_profile.h"
#include "game/map_header.h"
#include "game/player.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

extern u8 data_ov000_022dbd69[]; /* MapHeader.unk_41 of map 0 */
extern u8 data_02161b8c
    []; /* sets of 29 bytes: a mods id, then a template id per CO id */
extern u8 data_02161b8d[]; /* data_02161b8c + 1 */
extern u8 data_02161baa[]; /* data_02161b8c + 30: the template ids of set 1 */

#define GAME (data_027c027c->unk_48[data_027c0284])
#define SET_SIZE 29
#define MAP_AI_SET data_ov000_022dbd69[GAME->unk_1c4->mapId * sizeof(MapHeader)]

void AiProfile_RerollThreatLimits(AiProfile* profile, int modsId,
                                  int templateId);

/* Like AiProfile_BuildForPlayer, but always picks the template of the player's
 * front CO, and calls AiProfile_RerollThreatLimits, which rolls the unit values of the
 * template again (it takes a mods id it does not use). The AI calls it before
 * it uses a CO power. */
void AiProfile_RerandomiseForPlayer(int player) {
  if (MAP_AI_SET != 0) {
    AiProfile_RerollThreatLimits(
        gAiProfile, data_02161b8c[MAP_AI_SET * SET_SIZE],
        (data_02161b8d +
         MAP_AI_SET *
             SET_SIZE)[GAME->players[player].front.id & PLAYER_CO_ID_MASK]);
    return;
  }
  AiProfile_RerollThreatLimits(
      gAiProfile, data_02161b8c[SET_SIZE],
      data_02161baa[GAME->players[player].front.id & PLAYER_CO_ID_MASK]);
}
