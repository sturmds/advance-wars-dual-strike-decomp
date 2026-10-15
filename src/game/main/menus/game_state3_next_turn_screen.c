#include "types.h"
#include "game/owner_info.h"
#include "game/Unk_02080440.h"
#include "game/screen_other_map.h"
#include "game/menu_button.h"
#include "game/keypad.h"
#include "game/touch.h"
#include "game/song.h"

extern const MenuButtonTemplate gNextTurnButton;
extern const u16 gNextTurnBobOffsets[]; /* 0, 1, 2, 3, 3, 3, 2, 1, 0, 0 */
extern char gNextTurnTextEnglish[];     /* "NEXT TURN" */
extern char gNextTurnTextFrench[];
extern char gNextTurnTextGerman[];
extern char gNextTurnTextSpanish[];
extern char gNextTurnTextItalian[];

/* Id of the screen-wide button added for touch input. */
#define NEXT_TURN_BUTTON_ID 0xf0

/* y of the "NEXT TURN" line: `base` plus a bob of 0-3 pixels that steps
 * every 3 frames over a 30-frame cycle. */
#define NEXT_TURN_TEXT_Y(base) \
  (gNextTurnBobOffsets[(MainLoop_GetFrameCount() / 3) % 10] + (base))

int MainLoop_GetFrameCount(void);
void Turn_AdvanceToNextPlayer(void);
void MovementCost(void);
void func_020c4018(void);
void func_ov000_022b04ec(void);
void func_020ca9d8(void);
void func_020ba750(void);
void CurrentPlayer_StartTurn(void);
void CurrentPlayer_SyncState(void);
void Player_EndPower(int player);
BOOL HotSeat_HasSeveralHumanTeams(
    void); /* TRUE: two or more human teams share the DS */
int Player_GetNextInGame(
    int player); /* next player after `player` still in the game */
void NextTurn_DrawArmyName(int y, int army);
void Text_DrawCentered(int y, const char* text);

static inline BOOL Touch_IsPressed(void) {
  if (gTouch.active != 0 && gTouch.phase == TOUCH_PRESSED) {
    return TRUE;
  }
  return FALSE;
}

/* Handler for game state 3: ends the turn and goes to state 4. When human
 * players of different teams share the DS (HotSeat_HasSeveralHumanTeams), it first shows
 * the next army's name and a bobbing "NEXT TURN" on the top screen, and ends
 * the turn only once A is pressed or the touch screen is pressed. */
void Game_State3_NextTurnScreen(void) {
  if (HotSeat_HasSeveralHumanTeams()) {
    {
      Unk_027c0280* saved0280 = data_027c0280;
      Unk_02080440* saved027c;
      Screen_SetCurrentGfx(data_027c0288.unk_00);
      saved027c = data_027c027c;
      Screen_SetCurrent(data_027c0288.unk_00);
      if (MenuButton_Find(NEXT_TURN_BUTTON_ID) == NULL) {
        MenuButton_Add(0, 0, &gNextTurnButton, NEXT_TURN_BUTTON_ID);
      }
      Screen_SetCurrent(saved027c);
      Screen_SetCurrentGfx(saved0280);
    }
    {
      Unk_027c0280* saved0280 = data_027c0280;
      Screen_SetCurrentGfx(data_027c0288.unk_00);
      NextTurn_DrawArmyName(
          86, MAP_STATE->players[Player_GetNextInGame(MAP_STATE->currentPlayer)]
                  .armyColour);
      switch (data_ov000_023503a0) {
        case 0:
          Text_DrawCentered(NEXT_TURN_TEXT_Y(64), gNextTurnTextEnglish);
          break;
        case 1:
          Text_DrawCentered(NEXT_TURN_TEXT_Y(64), gNextTurnTextFrench);
          break;
        case 2:
          Text_DrawCentered(NEXT_TURN_TEXT_Y(64), gNextTurnTextGerman);
          break;
        case 3:
          Text_DrawCentered(NEXT_TURN_TEXT_Y(56), gNextTurnTextSpanish);
          break;
        case 4:
          Text_DrawCentered(NEXT_TURN_TEXT_Y(56), gNextTurnTextItalian);
          break;
      }
      Screen_SetCurrentGfx(saved0280);
    }
    if (!(gKeypad->pressed & PAD_BUTTON_A) && !Touch_IsPressed()) {
      return;
    }
    MenuButtons_CloseAll();
  }
  Turn_AdvanceToNextPlayer();
  CurrentPlayer_SyncState();
  CurrentPlayer_StartTurn();
  Player_EndPower(MAP_STATE->currentPlayer);
  MovementCost();
  func_020c4018();
  if (data_027c027c == data_027c0288.unk_00) {
    func_ov000_022b04ec();
  }
  if (HotSeat_HasSeveralHumanTeams()) {
    func_020ca9d8();
  }
  func_020ba750();
  Sound_FadeOutSong();
  MAP_STATE->unk_18 = 4;
}
