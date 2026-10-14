#include "types.h"
#include "game/ai_profile.h"
#include "game/map_header.h"
#include "game/player.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

extern u8 data_ov000_022dbd69[]; /* MapHeader.unk_41 of map 0 */
extern AiProfileSet data_02161b8c[];

#define GAME (data_027c027c->unk_48[data_027c0284])
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
        gAiProfile, data_02161b8c[MAP_AI_SET].modsId,
        data_02161b8c[MAP_AI_SET]
            .templateIds[GAME->players[player].front.id & PLAYER_CO_ID_MASK]);
    return;
  }
  AiProfile_RerollThreatLimits(
      gAiProfile, data_02161b8c[1].modsId,
      data_02161b8c[1]
          .templateIds[GAME->players[player].front.id & PLAYER_CO_ID_MASK]);
}
