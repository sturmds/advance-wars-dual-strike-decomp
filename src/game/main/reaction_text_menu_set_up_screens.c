#include "types.h"
#include "game/reaction_text_menu_task.h"

void Gfx_InitBothScreens(void);
void ModeFont_Load(int tile, int paletteSlot);
void ModeFont_ShowText(const void* text, int tile, int paletteSlot, int a,
                       int b);
const char* Text_Get(u32 textId);
void Wallpaper_LoadBoth(void);
void HelpBand_Start(int tile, int scrolls);
void HelpText_Show(u32 textId, int tile);

/* First script step of the reaction text menu: sets up both screens. */
void ReactionTextMenu_SetUpScreens(ReactionTextMenuTask* menu) {
  Gfx_InitBothScreens();
  ModeFont_Load(0, 0);
  ModeFont_ShowText(Text_Get(0x7d00001c), 0, 0, 0, 0);
  Wallpaper_LoadBoth();
  HelpBand_Start(0x3f8, 1);
  HelpText_Show(0x48000010, 960);
  menu->selected = 0;
  menu->done = 0;
}
