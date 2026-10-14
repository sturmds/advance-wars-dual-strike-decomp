#include "types.h"
#include "game/Unk_02080440.h"
#include "game/event_script.h"
#include "game/screen_other_map.h"
#include "game/script_slot.h"

extern int gCameraPresetIndex;
extern u8 data_021576fa[][20];
extern u8 data_021576fb[][20];
extern u32 gLinkPlay;
extern ScriptCommand gMapScrollScript[];

#define GAME (data_027c027c->unk_48[data_027c0284])

void Game_TickPlayerTimer(void);
void WeatherFx_UpdateClouds(void);
void MapScreen_UpdateAnimations(void);
int SceneMode_IsChanging(void);
int Map_IsThisMapsTurn(void);
s8 MenuDepth_Get(void);
s16 func_020b9914(void);
s16 ScriptSlot_Find(const ScriptCommand* script);
void SidePanel_Place(int mode);
int Link_GetOwnPlayer(void);
void MapView_ScrollStep(int step);
void AiTurn_Update(void);
void LinkAction_Update(void);
void func_ov000_022b0500(void);
void Game_State1_CheckDayLimit(void);
void Game_State2_LoadNextArmyIcon(void);
void Game_State3_NextTurnScreen(void);
void Game_State4_WaitSongFade(void);
void Game_State5_StartTurnTask(void);
void Game_State6_PlayThemeAndScripts(void);
void Game_State7_ScrollToPlayerCursor(void);
void Game_State8_PickNextState(void);
void Game_State9_RunStep(void);
void Game_State10_RunStep(void);
void Game_State11_SlidePanel(void);
void Game_State12_BlackHoleCheck(void);
void Game_State13_SlidePanelStartTask(void);
void Game_State14_PickTurnState(void);
void Game_State15_RunStep(void);
void Game_State18_RestorePrevious(void);
void Game_State20_CloseMap(void);
void Game_State22_ReopenPopup(void);

/* Copies the view scroll of map screen `from` to map screen `to` through the
 * caller's scrollX and scrollY, and puts the cursor tile (cursorX, cursorY) of
 * `to` at the middle of its view (halfW, halfH tiles in). */
#define COPY_VIEW_SCROLL(from, to)                         \
  {                                                        \
    Unk_027c0280* saved0280 = data_027c0280;               \
    Unk_02080440* saved027c;                               \
    Screen_SetCurrentGfx(from);                            \
    saved027c = data_027c027c;                             \
    Screen_SetCurrent(from);                               \
    scrollX = GAME->unk_1c4->scrollX;                      \
    scrollY = GAME->unk_1c4->scrollY;                      \
    Screen_SetCurrent(saved027c);                          \
    Screen_SetCurrentGfx(saved0280);                       \
  }                                                        \
  {                                                        \
    Unk_027c0280* saved0280 = data_027c0280;               \
    Unk_02080440* saved027c;                               \
    Screen_SetCurrentGfx(to);                              \
    saved027c = data_027c027c;                             \
    Screen_SetCurrent(to);                                 \
    GAME->unk_1c4->scrollX = scrollX;                      \
    GAME->unk_1c4->scrollY = scrollY;                      \
    GAME->cursorX = halfW + (GAME->unk_1c4->scrollX >> 4); \
    GAME->cursorY = halfH + (GAME->unk_1c4->scrollY >> 4); \
    Screen_SetCurrent(saved027c);                          \
    Screen_SetCurrentGfx(saved0280);                       \
  }

/* One frame of the map game. When the scene is not changing, it either runs
 * the handler of the current state (unk_18), if it is this map's turn, no
 * menu is open (menuDepth) and func_020b9914 returns 0, or, when it is the
 * other map's turn and unk_86 is set, makes this map's view follow the other
 * map's scroll. */
void Game_RunStateFrame(void) {
  int halfW;
  int halfH;
  s16 scrollX;
  s16 scrollY;

  if (data_027c027c == data_027c0288.unk_00) {
    Game_TickPlayerTimer();
  }
  WeatherFx_UpdateClouds();
  MapScreen_UpdateAnimations();

  if (!SceneMode_IsChanging()) {
    if (!Map_IsThisMapsTurn()) {
      if (GAME->unk_86 != 0) {
        halfW = data_021576fa[gCameraPresetIndex][0] >> 1;
        halfH = data_021576fb[gCameraPresetIndex][0] >> 1;
        if (data_027c027c == data_027c0288.unk_00) {
          COPY_VIEW_SCROLL(data_027c0288.unk_04, data_027c0288.unk_00);
        } else {
          COPY_VIEW_SCROLL(data_027c0288.unk_00, data_027c0288.unk_04);
        }
        MapView_ScrollStep(4);
      }
    } else if (MenuDepth_Get() == 0 && func_020b9914() == 0) {
      switch (GAME->unk_18) {
        case 1:
          Game_State1_CheckDayLimit();
          break;
        case 2:
          Game_State2_LoadNextArmyIcon();
          break;
        case 3:
          Game_State3_NextTurnScreen();
          break;
        case 4:
          Game_State4_WaitSongFade();
          break;
        case 5:
          Game_State5_StartTurnTask();
          break;
        case 6:
          Game_State6_PlayThemeAndScripts();
          break;
        case 7:
          Game_State7_ScrollToPlayerCursor();
          break;
        case 8:
          Game_State8_PickNextState();
          break;
        case 9:
          Game_State9_RunStep();
          break;
        case 12:
          Game_State12_BlackHoleCheck();
          break;
        case 10:
          Game_State10_RunStep();
          break;
        case 11:
          Game_State11_SlidePanel();
          break;
        case 13:
          Game_State13_SlidePanelStartTask();
          break;
        case 14:
          Game_State14_PickTurnState();
          break;
        case 15:
          Game_State15_RunStep();
          break;
        case 16:
          func_ov000_022b0500();
          if (gLinkPlay == 0 || Link_GetOwnPlayer() == 0) {
            AiTurn_Update();
          } else {
            LinkAction_Update();
          }
          break;
        case 21:
          func_ov000_022b0500();
          LinkAction_Update();
          break;
        case 18:
          Game_State18_RestorePrevious();
          break;
        case 20:
          Game_State20_CloseMap();
          break;
        case 22:
          Game_State22_ReopenPopup();
          return;
      }
    }
  }

  if (GAME->unk_18 == 16 || GAME->unk_1a == 16) {
    if (ScriptSlot_Find(gMapScrollScript) != -1 || MenuDepth_Get() == 0) {
      SidePanel_Place(SIDE_PANEL_SNAP_AWAY);
    }
  }
}
