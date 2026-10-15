#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/screen_other_map.h"

/* The 0x64 bytes of Unk_02080440_48 from gameMode (0x58) to 0xbc. */
typedef struct GameSettings {
  u32 data[0x19];
} GameSettings;

#define GAME_SETTINGS(game) (*(GameSettings*)&(game)->gameMode)

/* Copies the game settings (game mode and the fields after it) from the first
 * screen's game state to the second screen's. In game modes 1 and 2 the second
 * screen keeps its own slot controllers, and propertyGoal is cleared. */
void func_020f3ee4(void) {
  Unk_027c0280* savedGfx;
  Unk_02080440* savedScreen;
  GameSettings settings;
  u8 controllers[ARMY_SLOT_COUNT];
  int i;
  int j; /* needed to match: the second loop has its own counter */

  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  savedScreen = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  settings = GAME_SETTINGS(MAP_STATE);
  Screen_SetCurrent(savedScreen);
  Screen_SetCurrentGfx(savedGfx);

  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  savedScreen = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  for (i = 1; i < ARMY_SLOT_COUNT; i++) {
    controllers[i] = MAP_STATE->slotControllers[i];
  }
  GAME_SETTINGS(MAP_STATE) = settings;
  if (MAP_STATE->gameMode == GAME_MODE_WAR_ROOM || MAP_STATE->gameMode == 2) {
    for (j = 1; j < ARMY_SLOT_COUNT; j++) {
      MAP_STATE->slotControllers[j] = controllers[j];
    }
    MAP_STATE->propertyGoal = 0;
  }
  Screen_SetCurrent(savedScreen);
  Screen_SetCurrentGfx(savedGfx);
}
