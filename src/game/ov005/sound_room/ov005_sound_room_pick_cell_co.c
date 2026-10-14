#include "types.h"
#include "game/Unk_ov005_0236bbac.h"
#include "game/menu_button.h"
#include "game/screen_contexts.h"
#include "game/task.h"

int Rng_Next(void);

void Ov005SoundRoom_AllocWork(void);
void Ov005SoundRoom_FreeWork(void);
void Ov005SoundRoom_MoveCursorOnKeys(void);
void Ov005SoundRoom_OnATouched(void);
void Ov005SoundRoom_OnBTouched(void);
void Ov005SoundRoom_OnCellTouched(void);
void Ov005SoundRoom_OnLTouched(void);
void Ov005SoundRoom_OnRTouched(void);
void Ov005SoundRoom_OnXTouched(void);
void Ov005_SetUpSoundRoom(void);
void Ov005_UpdateSoundRoom(void);
extern u8 data_0217eb08[];
extern u8 data_02180b08[];
extern u8 data_ov005_0236ae9c[];
extern u8 data_0217fb08[];
extern u8 data_ov005_0236ae94[];
void func_ov005_023659b8(void);
extern u8 data_ov005_0236ae84[];
extern u8 data_ov005_0236ae8c[];
extern u8 data_ov005_0236aea4[];
extern u8 data_ov005_0236aeb0[];

static u8* data_ov005_0236af24[] = {
    (u8*)&data_ov005_0236ae9c, (u8*)&data_ov005_0236ae94,
    (u8*)&data_ov005_0236ae84, (u8*)&data_ov005_0236ae8c,
    (u8*)&data_ov005_0236aea4, (u8*)&data_ov005_0236aeb0};
/* declared `TilePair data_ov005_0236b054[]` in ov005_sound_room_draw_bar_meter.c: declared in src/game/ov005/sound_room/ov005_sound_room_draw_bar_meter.c, with a type no header defines */
u32 data_ov005_0236b054[] = {
#include "data/ov005/data_ov005_0236b054.inc.c"
};
MenuButtonTemplate data_ov005_0236b02c = {
#include "data/ov005/data_ov005_0236b02c.inc.c"
};
MenuButtonTemplate data_ov005_0236af64 = {
#include "data/ov005/data_ov005_0236af64.inc.c"
};
TaskScriptCmd data_ov005_0236afdc[] = {
#include "data/ov005/data_ov005_0236afdc.inc.c"
};
MenuButtonTemplate data_ov005_0236afb4 = {
#include "data/ov005/data_ov005_0236afb4.inc.c"
};
MenuButtonTemplate data_ov005_0236af8c = {
#include "data/ov005/data_ov005_0236af8c.inc.c"
};
/* declared `u8 data_ov005_0236b004[]` in ov005_sound_room_add_cell_buttons.c: relocation at data_ov005_0236b004+0x14 is not on a 4-byte field */
u32 data_ov005_0236b004[] = {
#include "data/ov005/data_ov005_0236b004.inc.c"
};
MenuButtonTemplate data_ov005_0236af3c = {
#include "data/ov005/data_ov005_0236af3c.inc.c"
};
u8 data_ov005_0236b07c[] = {
#include "data/ov005/data_ov005_0236b07c.inc.c"
};
/* declared `u8 data_ov005_0236b0bc[]` in ov005_set_up_sound_room.c: relocation at data_ov005_0236b0bc+0xc is not on a 4-byte field */
u32 data_ov005_0236b0bc[] = {
#include "data/ov005/data_ov005_0236b0bc.inc.c"
};

/* Sets state->coId from the cell under the cursor. Cell ids 0x80 to 0x85 stand
 * for a group of COs (table data_ov005_0236af24: a count, then the CO ids):
 * one is picked at random. */
void Ov005SoundRoom_PickCellCo(Unk_ov005_0236bbac* state) {
  int coId = state->cells[state->selection + state->page * 8].coId;

  switch (coId) {
    case 0x80:
    case 0x81:
    case 0x82:
    case 0x83:
    case 0x84:
    case 0x85: {
      u8* group = data_ov005_0236af24[coId - OV005_CO_RANDOM_FIRST];

      state->coId = group[(u32)Rng_Next() % group[0] + 1];
      break;
    }
    default:
      state->coId = coId;
      break;
  }
}
