#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern u16 data_0229d8c4;
u8 data_022a6b30;
/* also declared in src/game/main/main_loop_map_frame.c and 3 more */
u32 gMapGameRunning;
u8 data_022a6b34[0x4]; /* size: up to the next symbol */

void Panels_PlaceForCursor(void);
void func_020b9c3c(void);
void func_020b98bc(void);
void Units_ClearAll(void);
void MapObjects_Clear(void);
BOOL Map_IsCoSelectPhase(void);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Clears the map state's view and menu fields before a battle starts and sets
 * day 1, player 0. On a map where a player picks a CO first, the day starts at
 * 0. The name is a guess from the callers (battle setup hooks). */
void Battle_ResetTurnState(void) {
  STATE->cursorX = 0;
  STATE->cursorY = 0;
  STATE->cursorPixelX = 0;
  STATE->cursorPixelY = 0;
  Panels_PlaceForCursor();
  STATE->unk_10 = 0;
  STATE->unk_14 = 0;
  func_020b9c3c();
  func_020b98bc();
  data_0229d8c4 = 0;
  gMapGameRunning = 1;
  STATE->unk_18 = 4;
  STATE->unk_1c = 0;
  STATE->currentPlayer = 0;
  STATE->day = 1;
  Units_ClearAll();
  MapObjects_Clear();
  if (Map_IsCoSelectPhase()) {
    STATE->day = 0;
  }
}
