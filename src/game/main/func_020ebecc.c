#include "types.h"
#include "game/Unk_02080440.h"
#include "game/event_script.h"
#include "game/menu_button.h"
#include "game/song.h"

extern EventScriptCommand data_02168fd8[];
void Menu_CancelCallback(void);
void SongFadeOut_End(void);
void SongFadeOut_Step(void);
void SongRequest_Start(void);

/* in .data, so not const */
TaskScriptCmd gSongFadeOutScript[] = {
#include "data/main/gSongFadeOutScript.inc.c"
};
/* in .data, so not const */
TaskScriptCmd gSongRequestScript[] = {
#include "data/main/gSongRequestScript.inc.c"
};
static MenuButtonTemplate data_02158f7c = {
#include "data/main/data_02158f7c.inc.c"
};
/* declared `u32 gUnitSoundItems[UNIT_TYPE_COUNT * 2]` in sound_load_battle_unit_data.c: declared as one element of 0x4 bytes, but takes 0xd8 */
u32 gUnitSoundItems[] = {
#include "data/main/gUnitSoundItems.inc.c"
};

/* Starts the script at data_02168fd8, points the view target (cursorX, cursorY)
 * at the acting unit's destination tile and adds a menu button. Purpose
 * unknown. */
void func_020ebecc(void) {
  EventScript_Start(data_02168fd8);
  data_027c027c->unk_48[data_027c0284]->cursorX =
      data_027c027c->unk_48[data_027c0284]->actingUnitDestX;
  data_027c027c->unk_48[data_027c0284]->cursorY =
      data_027c027c->unk_48[data_027c0284]->actingUnitDestY;
  MenuButton_AddReserved(0, 0, &data_02158f7c);
}
