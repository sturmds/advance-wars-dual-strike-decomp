#include "types.h"
#include "game/Unk_ov006_0235f690.h"

int func_ov006_02357674(void);
void Sound_PlaySe(u32 soundId);

extern Unk_ov006_0235f690* data_ov006_0236d640;

/* Shows entry list page `page` while the editor idles (screen state 51):
 * unitPage (1-4) in unit mode, terrainPage (0-4) in terrain mode. Picking the page
 * already shown plays the refuse sound. */
void Ov006_SetEntryPage(int page) {
  Unk_ov006_0235f690* state = data_ov006_0236d640;

  if (state->screenState != 51) {
    return;
  }
  if (state->unitMode == 1) {
    if (state->unitPage != page) {
      state->unitPage = page;
      func_ov006_02357674();
      Sound_PlaySe(0x10001);
    } else {
      Sound_PlaySe(0x10008);
    }
  }
  state = data_ov006_0236d640;
  if (state->unitMode != 0) {
    return;
  }
  if (state->terrainPage != page) {
    state->prevTerrainPage = state->terrainPage;
    data_ov006_0236d640->terrainPage = page;
    func_ov006_02357674();
    Sound_PlaySe(0x10001);
  } else {
    Sound_PlaySe(0x10008);
  }
}
