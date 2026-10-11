#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

/* guess: TRUE when the tile holds a unit (MapTiles.unk_508) of a player with
 * Player.unk_1b bit 1 set */
BOOL func_020c9228(s16 x, s16 y);

#define GAME (data_027c027c->unk_48[data_027c0284])
#define MAP  (GAME->mapTiles)

/* TRUE when the unit at tile (x, y) can be shown. Same shape as
 * Unit_IsVisibleToCurrentPlayer, but tests Player.unk_1b bit 1 (doc 890: the
 * active player) where that one compares teams. Only dived units (Sub,
 * Stealth) can be hidden; they stay seen when their owner or the owner of
 * their tile has the bit, or when func_020c9228 finds a unit next to them. */
BOOL Unit_IsVisibleToActivePlayers(MapUnit *unit, s16 x, s16 y)
{
    u16 slot = unit - GAME->units;
    u16 count = 0;

    /* the original has this null test after it used the pointer */
    if (unit == NULL) {
        return TRUE;
    }
    if (!(unit->flags & MAP_UNIT_DIVED)) {
        return TRUE;
    }
    if (GAME->players[(slot >> 6) + 1].unk_1b & 2) {
        return TRUE;
    }
    if (GAME->players[MAP_TILE_OWNER(MAP->tiles[x + MAP->rowStart[y]])].unk_1b & 2) {
        return TRUE;
    }
    if (x > 0) {
        count += func_020c9228(x - 1, y);
    }
    if (y > 0) {
        count += func_020c9228(x, y - 1);
    }
    if (x < GAME->unk_1c4->mapWidth - 1) {
        count += func_020c9228(x + 1, y);
    }
    if (y < GAME->unk_1c4->mapHeight - 1) {
        count += func_020c9228(x, y + 1);
    }
    return count != 0;
}
