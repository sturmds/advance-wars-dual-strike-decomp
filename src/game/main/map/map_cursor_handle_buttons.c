#include "types.h"
#include "game/Unk_02080440.h"
#include "game/event_script.h"
#include "game/keypad.h"
#include "game/map_unit.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern const EventScriptCommand data_02169e38[];

void func_020d1420(void);
BOOL Map_IsCoSelectPhase(void);
void func_020bc5c0(void);
void func_020bc56c(void);
void func_020d1414(void);
void func_020f467c(void);
int func_020d9864(int x, int y);
void SecondScreen_ShowCursorInfo(int tile, MapUnit *unit);
MapUnit *func_020c92a0(void);
void func_020f5888(int x, int y);
s16 ScriptSlot_Find(const EventScriptCommand *script);
void ScreenConfig_RunActionHook(void);
BOOL func_020d0c4c(int x, int y, int a, int b);
void func_020d0f18(int x, int y);

#define GAME (data_027c027c->unk_48[data_027c0284])

/* Buttons on the map while nothing is selected. Start, Select, R (when
 * allowR) and X (when allowX) each start their own handler. Otherwise it
 * shows the cursor tile on the second screen, then L moves the cursor to
 * the unit func_020c92a0 picks, A selects at the cursor (func_020d0c4c)
 * and B runs func_020d0f18 there. Returns TRUE when a button was handled. */
BOOL MapCursor_HandleButtons(BOOL allowR, BOOL allowX)
{
    u16 keys = gKeypad->pressed;
    Unk_02080440_48 *game;
    MapTiles *tiles;
    int x;
    int y;
    MapUnit *unit;

    if (keys & PAD_BUTTON_START) {
        func_020d1420();
        return TRUE;
    }
    if (keys & PAD_BUTTON_SELECT) {
        if (Map_IsCoSelectPhase()) {
            func_020bc5c0();
        } else {
            func_020bc56c();
        }
        return TRUE;
    }
    if (allowR && (keys & PAD_BUTTON_R)) {
        func_020d1414();
        return TRUE;
    }
    if (allowX && (keys & PAD_BUTTON_X)) {
        func_020f467c();
        return TRUE;
    }
    game = GAME;
    y = game->unk_2a;
    x = game->unk_28;
    tiles = game->mapTiles;
    SecondScreen_ShowCursorInfo(func_020d9864(x, y), &game->units[tiles->unitAt[x + tiles->rowStart[y]]]);
    if (gKeypad->pressed & PAD_BUTTON_L) {
        unit = func_020c92a0();
        if (unit != NULL) {
            func_020f5888(unit->x, unit->y);
            if (ScriptSlot_Find(data_02169e38) != -1) {
                ScreenConfig_RunActionHook();
            }
            return TRUE;
        }
    }
    if (gKeypad->pressed & PAD_BUTTON_A) {
        if (Map_IsCoSelectPhase()) {
            func_020bc5c0();
            return TRUE;
        }
        if (func_020d0c4c(GAME->unk_28, GAME->unk_2a, 1, 1)) {
            return TRUE;
        }
    }
    if (!(gKeypad->pressed & PAD_BUTTON_B)) {
        return FALSE;
    }
    func_020d0f18(GAME->unk_28, GAME->unk_2a);
    return TRUE;
}
