#include "types.h"
#include "game/Unk_02080440.h"
#include "game/player.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

/* Campaign map lines: a CO power used on map (mapId - 224) starts the script
 * when the CO and the number of power uses match; player 0 means any player. */
typedef struct MapPowerLine {
    u8 player;
    s8 co;
    u8 powerUses;
    u8 pad_03;
    u32 script;
} MapPowerLine;

extern MapPowerLine *data_02166f9c[];

void CoQuote_StartText(u32 script);

#define GAME (data_027c027c->unk_48[data_027c0284])

/* Looks for a line of the current campaign map that fits the player's front CO
 * and power use count; starts it and returns TRUE when found. */
BOOL CoQuote_StartMapLine(int player)
{
    MapPowerLine *line = data_02166f9c[GAME->unk_1c4->mapId - 224];

    if (line == NULL) {
        return FALSE;
    }
    for (; line->co != -1; line++) {
        if (line->co == (GAME->players[player].front.id & PLAYER_CO_ID_MASK)
            && line->powerUses == GAME->players[player].front.powerUses
            && (line->player == 0 || line->player == player)) {
            CoQuote_StartText(line->script);
            return TRUE;
        }
    }
    return FALSE;
}
