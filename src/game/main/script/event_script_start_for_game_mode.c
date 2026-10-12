#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern u8 data_02166b08[];
extern u8 data_02166b88[];

#define STATE (data_027c027c->unk_48[data_027c0284])

void* EventScript_Start(const void* script);

/* Starts the event script for the game mode: one script in Survival, another
 * in every other mode. */
void EventScript_StartForGameMode(void) {
  EventScript_Start(STATE->gameMode == GAME_MODE_SURVIVAL ? data_02166b88
                                                          : data_02166b08);
}
