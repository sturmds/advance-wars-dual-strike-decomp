#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/main_loop.h"
#include "game/screen_other_map.h"
#include "sdk/svc.h"

extern MainLoop gMainLoop;

int func_01ff9f0c(void);
void* func_01ff9ee4(void);
void* func_01ff9ef8(void);
void func_01ff9f1c(int value);
void MainLoop_CheckResetCombo(void);
void MainLoop_UpdateSleep(void);
void GfxQueue_FreeOwnedCopies(void);
void Touch_Poll(void);
void Keypad_Poll(void);
void Link_HandlePackets(void);
void Sound_Update(void);
void OamBuffer_ClearTail(void);
void PlayRecords_CountFrame(void);
void Link_FlushSendQueue(void);
void MenuButtons_Update(void);
void SpriteList_Draw(int list);
void Sprites_ResetLists(void);
void* MainLoop_CountFrame(void);

/* Runs `body` with `screen` as the current screen, then restores the
 * previous one. */
#define ON_SCREEN(screen, body)  \
  savedGfx = data_027c0280;      \
  Screen_SetCurrentGfx(screen);  \
  savedState = data_027c027c;    \
  Screen_SetCurrent(screen);     \
  body;                          \
  Screen_SetCurrent(savedState); \
  Screen_SetCurrentGfx(savedGfx)

/* One frame of the main loop, called by main. Does nothing while the
 * last one has not been sent to the hardware yet (func_01ff9f0c, cleared
 * by MainLoop_DefaultVBlank). Otherwise reads input, link packets and
 * sound, runs the per-screen callbacks, builds both screens' sprite lists,
 * marks the frame ready and waits for VBlank. */
void MainLoop_RunFrame(void) {
  Unk_027c0280* savedGfx;
  Unk_02080440* savedState;

  if (func_01ff9f0c()) {
    return;
  }
  func_01ff9ee4();
  GfxQueue_FreeOwnedCopies();
  Touch_Poll();
  Keypad_Poll();
  MainLoop_CheckResetCombo();
  MainLoop_UpdateSleep();
  Link_HandlePackets();
  Sound_Update();
  ON_SCREEN(data_027c0288.unk_00, OamBuffer_ClearTail());
  ON_SCREEN(data_027c0288.unk_04, OamBuffer_ClearTail());
  if (gMainLoop.screenCallbacks[0] != NULL) {
    ON_SCREEN(data_027c0288.unk_00, gMainLoop.screenCallbacks[0]());
  }
  if (gMainLoop.screenCallbacks[1] != NULL) {
    ON_SCREEN(data_027c0288.unk_04, gMainLoop.screenCallbacks[1]());
  }
  PlayRecords_CountFrame();
  Link_FlushSendQueue();
  MenuButtons_Update();
  ON_SCREEN(data_027c0288.unk_00, SpriteList_Draw(0); SpriteList_Draw(5);
            Sprites_ResetLists());
  ON_SCREEN(data_027c0288.unk_04, SpriteList_Draw(0); SpriteList_Draw(5);
            Sprites_ResetLists());
  MainLoop_CountFrame();
  func_01ff9ef8();
  func_01ff9f1c(1);
  SVC_WaitVBlankIntr();
}
