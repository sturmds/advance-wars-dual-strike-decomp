#include "types.h"
#include "game/Unk_02088d04.h"

extern int gNextGameMode; /* next game mode, set by MainMenu_ConfirmEntry */

int func_02102378(void);
s8 func_02102394(void);
s8 func_021023a4(void);
s8 func_021023b4(void);
int func_021023c4(void);
s8 func_021023d4(void);
s8 func_021023e4(void);
void Gfx_LoadPlayerWindowFrame(int player);
void EventScript_StartText(u32 script, int arg1, int arg2);

/* Runs when a main menu entry picks game mode gNextGameMode. For some
 * modes, if that mode's getter (all three for modes 0 and 1) returns
 * nonzero, shows BG0-3 and sprites, turns the windows off and starts event
 * script 0xc00002cf. What the getters track is not known yet. */
void func_0202d6c0(void) {
  switch (gNextGameMode) {
    case 0:
    case 1:
      if (!func_021023e4()) {
        return;
      }
      if (!func_021023d4()) {
        return;
      }
      if (!func_021023c4()) {
        return;
      }
      break;
    case 5:
    case 6:
      if (!func_021023b4()) {
        return;
      }
      break;
    case 14:
    case 15:
    case 16:
      if (!func_02102378()) {
        return;
      }
      break;
    case 12:
      if (!func_021023a4()) {
        return;
      }
      break;
    case 8:
    case 9:
    case 10:
      if (!func_02102394()) {
        return;
      }
      break;
    default:
      return;
  }
  GFX_DISPCNT(data_027c0280)->bg0 = 1;
  GFX_DISPCNT(data_027c0280)->bg1 = 1;
  GFX_DISPCNT(data_027c0280)->bg2 = 1;
  GFX_DISPCNT(data_027c0280)->bg3 = 1;
  GFX_DISPCNT(data_027c0280)->obj = 1;
  GFX_DISPCNT(data_027c0280)->win0 = 0;
  GFX_DISPCNT(data_027c0280)->win1 = 0;
  GFX_DISPCNT(data_027c0280)->objWin = 0;
  Gfx_LoadPlayerWindowFrame(0);
  EventScript_StartText(0xc00002cf, 21, 0);
}
