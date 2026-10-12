#include "types.h"
#include "game/player.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

u8 Map_GetLoadedPlayerCount(void);
void Number_Draw(s16 x, s16 y, u16* map, u32 value, u16 entryBits,
                 u16 digitSet);
void Text_PrintImmediate(s16 x, s16 y, u16* map, u32 textId, u16 unk_30,
                         int arg);
void Text_PrintStringImmediate(s16 x, s16 y, u16* map, const char* text, u16 a,
                               u16 b);
const char* Map_GetCurrentName(void);
int Player_CountUnitsBuilt(int player); /* sum of Player.unitsBuilt */
int Player_CountUnitsLost(int player);  /* sum of Player.unitsLost */

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern int data_027c0284;
extern char data_02167c70[]; /* "?" */
extern char data_02167c8c[]; /* shown for a defeated player */

#define GAME (data_027c027c->unk_48[data_027c0284])

/* Status screen table on layer 0: map name and day on row 4, column headers
 * on row 8, then one row per player from row 10: units built, units lost,
 * properties, income and funds. In fog or rain, other teams' properties and
 * income show as "?" unless propertyGoal is set, and their funds always do. Row 18
 * shows the neutral properties. */
void StatusScreen_Draw(void) {
  int i;
  int y;
  Player* player;
  Unk_02080440_48* game;
  u16* map;

  map = data_027c0280->layer0Map;
  Text_PrintStringImmediate(3, 4, map, Map_GetCurrentName(), 0, 0);
  Number_Draw(30, 4, data_027c0280->layer0Map, GAME->day, 0, 0);
  if (GAME->day >= 100) {
    Text_PrintImmediate(25, 4, data_027c0280->layer0Map, 0xc00000ad, 0, 0);
  } else if (GAME->day >= 10) {
    Text_PrintImmediate(26, 4, data_027c0280->layer0Map, 0xc00000ad, 0, 0);
  } else {
    Text_PrintImmediate(27, 4, data_027c0280->layer0Map, 0xc00000ad, 0, 0);
  }
  Text_PrintImmediate(6, 8, data_027c0280->layer0Map, 0xc00000a3, 0, 0);
  Text_PrintImmediate(10, 8, data_027c0280->layer0Map, 0xc00000a4, 0, 0);
  Text_PrintImmediate(14, 8, data_027c0280->layer0Map, 0xc00000b3, 0, 0);
  Text_PrintImmediate(18, 8, data_027c0280->layer0Map, 0xc00000ae, 0, 0);
  Text_PrintImmediate(25, 8, data_027c0280->layer0Map, 0xc00000b4, 0, 0);
  Text_PrintImmediate(1, 18, data_027c0280->layer0Map, 0xc00000a9, 0, 0);

  for (i = 0; i < Map_GetLoadedPlayerCount(); i++) {
    y = 10 + i * 2;
    /* The original tests player 1's controller on every row. */
    if (GAME->players[1].controller == PLAYER_CONTROLLER_NONE) {
      continue;
    }
    map = data_027c0280->layer0Map;
    Number_Draw(8, y, map, Player_CountUnitsBuilt(i + 1), 0, 0);
    map = data_027c0280->layer0Map;
    Number_Draw(12, y, map, Player_CountUnitsLost(i + 1), 0, 0);
    game = GAME;
    player = &game->players[i + 1];
    if (player->defeated) {
      Text_PrintImmediate(2, y, data_027c0280->layer0Map, 0xc00000a4, 0, 0);
      Text_PrintStringImmediate(16, y, data_027c0280->layer0Map, data_02167c8c,
                                0, 0);
      Text_PrintStringImmediate(23, y, data_027c0280->layer0Map, data_02167c8c,
                                0, 0);
      Text_PrintStringImmediate(30, y, data_027c0280->layer0Map, data_02167c8c,
                                0, 0);
    } else if (
        (game->unk_1c4->fog == 0 && game->unk_1c4->weather != WEATHER_RAIN)
        /* needed to match: the volatile read makes the compiler load players again */
        || player->team ==
               (*(Player* volatile*)&game->players)[game->currentPlayer].team) {
      Number_Draw(16, y, data_027c0280->layer0Map, player->propertyCount, 0, 0);
      Number_Draw(23, y, data_027c0280->layer0Map, GAME->players[i + 1].unk_08,
                  0, 0);
      Number_Draw(30, y, data_027c0280->layer0Map, GAME->players[i + 1].funds,
                  0, 0);
    } else {
      if (game->propertyGoal) {
        Number_Draw(16, y, data_027c0280->layer0Map, player->propertyCount, 0,
                    0);
        Number_Draw(23, y, data_027c0280->layer0Map,
                    GAME->players[i + 1].unk_08, 0, 0);
      } else {
        Text_PrintStringImmediate(16, y, data_027c0280->layer0Map,
                                  data_02167c70, 0, 0);
        Text_PrintStringImmediate(23, y, data_027c0280->layer0Map,
                                  data_02167c70, 0, 0);
      }
      Text_PrintStringImmediate(30, y, data_027c0280->layer0Map, data_02167c70,
                                0, 0);
    }
  }

  game = GAME;
  if ((game->unk_1c4->fog == 0 && game->unk_1c4->weather != WEATHER_RAIN) ||
      game->propertyGoal) {
    Number_Draw(16, 18, data_027c0280->layer0Map,
                game->players[0].propertyCount, 0, 0);
  } else {
    Text_PrintStringImmediate(16, 18, data_027c0280->layer0Map, data_02167c70,
                              0, 0);
  }
}
