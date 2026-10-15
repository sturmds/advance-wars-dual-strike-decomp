#include "types.h"
#include "game/Unk_02080440.h"
#include "game/event_script.h"
#include "game/keypad.h"
#include "game/map_unit.h"
#include "game/script_slot.h"
#include "game/screen.h"

extern ScriptCommand gMapScrollScript[];

void func_020d1420(void);
BOOL Map_IsCoSelectPhase(void);
void func_020bc5c0(void);
void func_020bc56c(void);
void func_020d1414(void);
int Map_GetDisplayTile(int x, int y);
void SecondScreen_ShowCursorInfo(int tile, MapUnit* unit);
MapUnit* CurrentPlayer_NextIdleUnit(void);
void func_020f5888(int x, int y);
s16 ScriptSlot_Find(const ScriptCommand* script);
void ScreenConfig_RunActionHook(void);
BOOL MapCursor_SelectTile(int x, int y, int a, int b);
void MapCursor_ShowRange(int x, int y);

/* Buttons on the map while nothing is selected. Start, Select, R (when
 * allowR) and X (when allowX) each start their own handler. Otherwise it
 * shows the cursor tile on the second screen, then L moves the cursor to
 * the unit CurrentPlayer_NextIdleUnit picks, A selects at the cursor (MapCursor_SelectTile)
 * and B runs MapCursor_ShowRange there. Returns TRUE when a button was handled. */
BOOL MapCursor_HandleButtons(BOOL allowR, BOOL allowX) {
  u16 keys = gKeypad->pressed;
  Unk_02080440_48* game;
  MapTiles* tiles;
  int x;
  int y;
  MapUnit* unit;

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
    SceneMode_OnButtonX();
    return TRUE;
  }
  game = MAP_STATE;
  y = game->cursorY;
  x = game->cursorX;
  tiles = game->mapTiles;
  SecondScreen_ShowCursorInfo(
      Map_GetDisplayTile(x, y),
      &game->units[tiles->unitAt[x + tiles->rowStart[y]]]);
  if (gKeypad->pressed & PAD_BUTTON_L) {
    unit = CurrentPlayer_NextIdleUnit();
    if (unit != NULL) {
      func_020f5888(unit->x, unit->y);
      if (ScriptSlot_Find(gMapScrollScript) != -1) {
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
    if (MapCursor_SelectTile(MAP_STATE->cursorX, MAP_STATE->cursorY, 1, 1)) {
      return TRUE;
    }
  }
  if (!(gKeypad->pressed & PAD_BUTTON_B)) {
    return FALSE;
  }
  MapCursor_ShowRange(MAP_STATE->cursorX, MAP_STATE->cursorY);
  return TRUE;
}
