#include "types.h"
#include "game/event_script.h"
#include "game/Unk_02080440.h"

extern EventScriptCommand data_02166b08[];
extern EventScriptCommand data_02166b88[];

/* Starts the event script for the game mode: one script in Survival, another
 * in every other mode. */
void EventScript_StartForGameMode(void) {
  EventScript_Start(MAP_STATE->gameMode == GAME_MODE_SURVIVAL ? data_02166b88
                                                              : data_02166b08);
}
