#include "types.h"
#include "game/Unk_02080440.h"
#include "game/event_script.h"
#include "game/player.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern u32 gLinkPlay;
extern const EventScriptCommand gMapScrollScript[];

#define STATE (data_027c027c->unk_48[data_027c0284])

BOOL EventScript_IsRunning(void);
s16 ScriptSlot_Find(const EventScriptCommand *script);
void Font_LoadDigits(int baseTile);
void func_020f5888(int x, int y);
void Player_PlayTheme(int player);
void Panels_PlaceForCursor(void);
void Weather_StartNext(void);
void Units_WaitParalysed(void);
void func_020c4018(void);

/* Handler for game state 7. Waits for event scripts and the map scroll.
 * Scrolls to the current player's saved cursor tile, except for a player
 * that is not human while gLinkPlay (likely link play) is set, and waits
 * for that scroll.
 * Then plays the player's theme, starts the weather and switches to state 8. */
void Game_State7_ScrollToPlayerCursor(void)
{
    if (EventScript_IsRunning()) {
        return;
    }
    if (ScriptSlot_Find(gMapScrollScript) != -1) {
        return;
    }
    Font_LoadDigits(0);
    if (gLinkPlay == 0
        || STATE->players[STATE->currentPlayer].controller == PLAYER_CONTROLLER_HUMAN) {
        func_020f5888(STATE->players[STATE->currentPlayer].savedCursorX,
                      STATE->players[STATE->currentPlayer].savedCursorY);
    }
    if (ScriptSlot_Find(gMapScrollScript) != -1) {
        return;
    }
    Player_PlayTheme((u8)STATE->currentPlayer);
    Panels_PlaceForCursor();
    Weather_StartNext();
    Units_WaitParalysed();
    func_020c4018();
    STATE->unk_18 = 8;
}
