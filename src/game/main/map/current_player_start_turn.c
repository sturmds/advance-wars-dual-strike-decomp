#include "types.h"
#include "game/player.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

void CurrentPlayer_CollectIncome(void);
BOOL Player_HasHq(int player);
void MapCursor_SetTile(int x, int y, s16 *tile, u16 *pixel);
void CurrentPlayer_LoadPalettes(void);

#define GAME (data_027c027c->unk_48[data_027c0284])
#define PLAYER (GAME->players[GAME->currentPlayer])

/* Turn start for the current player: clears the per-turn kill count, the
 * front CO's powerReady bits and the tag power half, collects income, and
 * puts the map cursor on the player's saved tile (unk_31, unk_32). On day 0
 * or 1 that tile is first set to the HQ, or to the middle of the view when
 * the player has no HQ. */
void CurrentPlayer_StartTurn(void)
{
    GAME->cycleUnitIndex = 0;
    PLAYER.turnKills = 0;
    PLAYER.front.powerReady = 0;
    PLAYER.tagPowerHalf = 0;
    CurrentPlayer_CollectIncome();
    if (GAME->day <= 1) {
        /* needed to match: the s16 casts (probably s16 tile locals in the original) */
        if (Player_HasHq(GAME->currentPlayer)) {
            MapCursor_SetTile((s16)(PLAYER.hqX & 0x7f), (s16)(PLAYER.hqY & 0x7f), &GAME->cursorX, &GAME->cursorPixelX);
        } else {
            MapCursor_SetTile((s16)(GAME->unk_1c4->scrollX / 16 + 7), (s16)(GAME->unk_1c4->scrollY / 16 + 4),
                          &GAME->cursorX, &GAME->cursorPixelX);
        }
        PLAYER.unk_31 = GAME->cursorX;
        PLAYER.unk_32 = GAME->cursorY;
    }
    MapCursor_SetTile(PLAYER.unk_31, PLAYER.unk_32, &GAME->cursorX, &GAME->cursorPixelX);
    CurrentPlayer_LoadPalettes();
}
