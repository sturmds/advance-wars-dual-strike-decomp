#include "types.h"

void Wallpaper_DestroyTasks(void);
void SecondScreenTasks_Destroy(void);
void UnitInfoPanel_Close(void);
void Sprites_ResetLists(void);
void OamBuffer_Split(int count);

void ScreenOverlays_Destroy(void) {
  Wallpaper_DestroyTasks();
  SecondScreenTasks_Destroy();
  UnitInfoPanel_Close();
  Sprites_ResetLists();
  OamBuffer_Split(16);
}
