#include "types.h"
#include "game/player.h"
#include "game/Unk_02080440.h"

void CurrentPlayer_CollectIncome(void);
BOOL Player_HasHq(int player);
void MapCursor_SetTile(int x, int y, s16* tile, u16* pixel);
void CurrentPlayer_LoadPalettes(void);

#define PLAYER (MAP_STATE->players[MAP_STATE->currentPlayer])

/* Turn start for the current player: clears the per-turn kill count, the
 * front CO's powerReady bits and the tag power half, collects income, and
 * puts the map cursor on the player's saved tile (savedCursorX, savedCursorY). On day 0
 * or 1 that tile is first set to the HQ, or to the middle of the view when
 * the player has no HQ. */
void CurrentPlayer_StartTurn(void) {
  MAP_STATE->cycleUnitIndex = 0;
  PLAYER.turnKills = 0;
  PLAYER.front.powerReady = 0;
  PLAYER.tagPowerHalf = 0;
  CurrentPlayer_CollectIncome();
  if (MAP_STATE->day <= 1) {
    /* needed to match: the s16 casts (probably s16 tile locals in the original) */
    if (Player_HasHq(MAP_STATE->currentPlayer)) {
      MapCursor_SetTile((s16)(PLAYER.hqX & 0x7f), (s16)(PLAYER.hqY & 0x7f),
                        &MAP_STATE->cursorX, &MAP_STATE->cursorPixelX);
    } else {
      MapCursor_SetTile((s16)(MAP_STATE->unk_1c4->scrollX / 16 + 7),
                        (s16)(MAP_STATE->unk_1c4->scrollY / 16 + 4),
                        &MAP_STATE->cursorX, &MAP_STATE->cursorPixelX);
    }
    PLAYER.savedCursorX = MAP_STATE->cursorX;
    PLAYER.savedCursorY = MAP_STATE->cursorY;
  }
  MapCursor_SetTile(PLAYER.savedCursorX, PLAYER.savedCursorY,
                    &MAP_STATE->cursorX, &MAP_STATE->cursorPixelX);
  CurrentPlayer_LoadPalettes();
}
