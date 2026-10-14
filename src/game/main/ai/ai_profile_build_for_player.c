#include "types.h"
#include "game/ai_profile.h"
#include "game/map_header.h"
#include "game/player.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern Unk_027c0288 data_027c0288;

extern u8 data_ov000_022dbd69[]; /* MapHeader.unk_41 of map 0 */
extern AiProfileSet data_02161b8c[];

#define GAME (data_027c027c->unk_48[data_027c0284])
#define CURRENT_MAP ((Unk_02080440*)(&data_027c0288.unk_00)[data_027c0284])

/* Each map names a set in MapHeader.unk_41 (0: none, then set 1 is used). */

#define MAP_AI_SET data_ov000_022dbd69[GAME->unk_1c4->mapId * sizeof(MapHeader)]

void AiProfile_Build(AiProfile* profile, int modsId, int templateId);

/* Builds gAiProfile for a player. The mods come from the map's set. The
 * template is the front CO's from the set when CO effects are on, the
 * CO's from set 1 when the current player is human in the other screen's
 * state (game mode 0), and otherwise the set's first. */
void AiProfile_BuildForPlayer(int player) {
  if (MAP_AI_SET != 0) {
    if (GAME->coEffectsOn != 0) {
      AiProfile_Build(
          gAiProfile, data_02161b8c[MAP_AI_SET].modsId,
          data_02161b8c[MAP_AI_SET]
              .templateIds[GAME->players[player].front.id & PLAYER_CO_ID_MASK]);
      return;
    }
    if (GAME->gameMode == GAME_MODE_CAMPAIGN &&
        CURRENT_MAP->unk_48[data_027c0284]
                ->players[GAME->currentPlayer]
                .controller == PLAYER_CONTROLLER_HUMAN) {
      AiProfile_Build(
          gAiProfile, data_02161b8c[MAP_AI_SET].modsId,
          data_02161b8c[1]
              .templateIds[GAME->players[player].front.id & PLAYER_CO_ID_MASK]);
      return;
    }
    AiProfile_Build(gAiProfile, data_02161b8c[MAP_AI_SET].modsId,
                    data_02161b8c[MAP_AI_SET].templateIds[0]);
    return;
  }
  AiProfile_Build(
      gAiProfile, data_02161b8c[1].modsId,
      data_02161b8c[1]
          .templateIds[GAME->players[player].front.id & PLAYER_CO_ID_MASK]);
}
