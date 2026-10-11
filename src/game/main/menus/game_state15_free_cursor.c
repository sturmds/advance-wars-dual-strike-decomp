#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

int SceneMode_IsCurrent(int a0);
BOOL func_020d194c(void);
u32 MapTouch_ReadButtons(int rightButton, int leftButton);
void MapScreen_DrawCenterButton(void);
void MapScreen_DrawRightButton(void);
void MapScreen_DrawLeftButton(void);
void func_020c4024(void);
void func_020c407c(void);
void MapView_ScrollStep(int directions);
void Map_ScrollViewToTile(int mode);
void ScreenConfig_RunActionHook(void);
BOOL MapView_IsAtRest(void);
int MapScript_RunList18(int flag);
BOOL Survival_IsBudgetSpent(void);
void func_020dd3b4(void);
BOOL MapCursor_HandleButtons(BOOL allowR, BOOL allowX);
BOOL MapTouch_HandleTileTap(void);
BOOL Map_IsCoSelectPhase(void);
void func_020bc5c0(void);
void func_020bc56c(void);
void func_020d1414(void);
int SceneMode_OnButtonX(void);
void MapScreen_DrawInfoPanel(void);
void SidePanel_Place(int mode);

/* What MapTouch_ReadButtons returns. */
#define TAP_NONE          0
#define TAP_CENTER_BUTTON 0x10
#define TAP_MAP           0x20
#define TAP_LEFT_BUTTON   0x40
#define TAP_RIGHT_BUTTON  0x100

#define GAME (data_027c027c->unk_48[data_027c0284])

/* Game_State15_RunStep sub-state 0: the free map cursor. The right touch
 * button (unit info, func_020d1414) is offered only when SceneMode_IsCurrent(5)
 * holds and a unit stands under the cursor; the left one (SceneMode_OnButtonX)
 * when func_020d194c says so. Draws the buttons while the screen is not
 * touched, scrolls the view, then, once the view is at rest and no map
 * script or Survival budget check takes over, hands the buttons and touch
 * to their handlers. Otherwise it draws the info and side panels. */
void Game_State15_FreeCursor(void)
{
    BOOL rightButton = FALSE;
    BOOL leftButton;
    BOOL handled;

    if (SceneMode_IsCurrent(5)) {
        if (GAME->mapTiles->unitAt[GAME->mapTiles->rowStart[GAME->cursorY] + GAME->cursorX] != 0) {
            rightButton = TRUE;
        }
    }
    leftButton = func_020d194c();
    switch (MapTouch_ReadButtons(rightButton, leftButton)) {
    case TAP_NONE:
        MapScreen_DrawCenterButton();
        if (rightButton) {
            MapScreen_DrawRightButton();
        }
        if (leftButton) {
            MapScreen_DrawLeftButton();
        }
        func_020c4024();
        func_020c407c();
        break;
    case TAP_MAP:
        func_020c4024();
        func_020c407c();
        break;
    }
    MapView_ScrollStep(4);
    Map_ScrollViewToTile(0);
    ScreenConfig_RunActionHook();
    if (MapView_IsAtRest()) {
        if (MapScript_RunList18(1)) {
            return;
        }
        if (Survival_IsBudgetSpent()) {
            func_020dd3b4();
            return;
        }
        /* needed to match: the int cast gives signed compares */
        switch ((int)MapTouch_ReadButtons(rightButton, leftButton)) {
        case TAP_NONE:
        case TAP_MAP:
            handled = MapCursor_HandleButtons(rightButton, leftButton);
            if (!handled) {
                handled = MapTouch_HandleTileTap();
            }
            if (handled) {
                return;
            }
            break;
        case TAP_CENTER_BUTTON:
            if (Map_IsCoSelectPhase()) {
                func_020bc5c0();
            } else {
                func_020bc56c();
            }
            return;
        case TAP_RIGHT_BUTTON:
            func_020d1414();
            return;
        case TAP_LEFT_BUTTON:
            SceneMode_OnButtonX();
            return;
        }
    }
    MapScreen_DrawInfoPanel();
    SidePanel_Place(0);
}
