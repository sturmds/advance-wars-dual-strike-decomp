#include "types.h"
#include "game/Unk_02080440.h"

void SpriteTask_DestroyAllFirstScreen(void);
void func_020828c0(void);
void Wallpaper_DestroyTasks(void);
void Screen1_DestroyTextTasks(void);
int func_0207a260(void);
void func_0207bd58(int player);
u8 func_020b983c(void);
void func_0207bda8(int value);
void CombatSetup_StartLink(void);
void func_020d6a1c(void);
void func_0207dc60(void);
void Link_Stop(void);
void func_02103714(void);

/* Closes the link screens and stores the own player number. When func_020b983c returns 1, starts the
 * next stage by link mode (unk_8b): mode 3 the combat setup, modes 0 to 2 their own screens, mode 4
 * stops the link. With that flag clear the link stops too. */
void LinkMenu_EndScreen(void) {
  SpriteTask_DestroyAllFirstScreen();
  func_020828c0();
  Wallpaper_DestroyTasks();
  Screen1_DestroyTextTasks();
  func_0207bd58(func_0207a260());
  if (func_020b983c() == 1) {
    switch (MAP_STATE->unk_8b) {
      case 3:
        func_0207bda8(3);
        CombatSetup_StartLink();
        return;
      case 0:
      case 1:
        func_020d6a1c();
        return;
      case 2:
        func_0207dc60();
        return;
      case 4:
      default:
        func_0207bda8(3);
        Link_Stop();
        func_02103714();
        return;
    }
  }
  Link_Stop();
  func_02103714();
}
