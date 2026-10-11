#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

void Screen_SetCurrent(void *state);
void Screen_SetCurrentGfx(void *state);

extern Unk_02080440 *data_027c027c;
extern Unk_027c0280 *data_027c0280;
extern int data_027c0284;
extern Unk_027c0288 data_027c0288;

#define GAME (data_027c027c->unk_48[data_027c0284])

/* The 0x64 bytes of Unk_02080440_48 from gameMode (0x58) to 0xbc. */
typedef struct GameSettings {
    u32 data[0x19];
} GameSettings;

#define GAME_SETTINGS(game) (*(GameSettings *)&(game)->gameMode)

/* Copies the game settings (game mode and the fields after it) from the first
 * screen's game state to the second screen's. In game modes 1 and 2 the second
 * screen keeps its own slot controllers, and propertyGoal is cleared. */
void func_020f3ee4(void)
{
    Unk_027c0280 *savedGfx;
    Unk_02080440 *savedScreen;
    GameSettings settings;
    u8 controllers[5];
    int i;
    int j; /* needed to match: the second loop has its own counter */

    savedGfx = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_00);
    savedScreen = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_00);
    settings = GAME_SETTINGS(GAME);
    Screen_SetCurrent(savedScreen);
    Screen_SetCurrentGfx(savedGfx);

    savedGfx = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_04);
    savedScreen = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_04);
    for (i = 1; i < 5; i++) {
        controllers[i] = GAME->slotControllers[i];
    }
    GAME_SETTINGS(GAME) = settings;
    if (GAME->gameMode == GAME_MODE_WAR_ROOM || GAME->gameMode == 2) {
        for (j = 1; j < 5; j++) {
            GAME->slotControllers[j] = controllers[j];
        }
        GAME->propertyGoal = 0;
    }
    Screen_SetCurrent(savedScreen);
    Screen_SetCurrentGfx(savedGfx);
}
