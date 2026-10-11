#include "types.h"
#include "game/event_script.h"
#include "game/Unk_02080440.h"
#include "game/song.h"

extern u8 gEventSkipState;
extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
int Player_GetCoThemeSong(int player);

/* Script command: requests the theme of the current player's CO. When
 * gEventSkipState is 1 it only advances. The (u8) cast is needed to match. */
s16 EventScript_PlayCurrentCoTheme(s16 index)
{
    int done;

    if (gEventSkipState == 1) {
        done = TRUE;
    } else {
        Sound_RequestSong(Player_GetCoThemeSong((u8)data_027c027c->unk_48[data_027c0284]->currentPlayer));
        done = FALSE;
    }
    data_022977f0[index].pc++;
    return done;
}
