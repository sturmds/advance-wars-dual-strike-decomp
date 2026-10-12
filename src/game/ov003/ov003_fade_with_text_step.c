#include "types.h"
#include "game/screen_other_map.h"
#include "game/Unk_02160abc.h"
#include "game/Unk_ov003_0235dd38.h"
#include "game/ov003_unit.h"

const void* Text_Get(u32 textId);
int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void Task_ResumeScript(void* task);
int strlen(const char* str);
int Ov003_GetTextWidth(const u8* str);
int Ov003_DrawLetterAlt(int ch, u16 x, int y);

/* Task step: while Ov003State.unk_1d7 is 0, counts 48 frames and fades both
 * screens with master brightness. If data_02160abc->unk_0a equals the row of
 * the current slot, it draws text 0x7d000023 centred at y 80 each frame and
 * fades to white; otherwise it fades to black. Then it resumes the task script. */
void Ov003_FadeWithTextStep(Unk_ov003_0235dd38* obj) {
  int i;
  int x;
  Unk_027c0280* savedGfx;
  Unk_02080440* savedScreen;
  int brightness;

  if (data_ov003_02364928->unk_1d7 == 0) {
    obj->frame++;
  }
  if (data_02160abc->unk_0a ==
      data_ov003_02364928->slots[data_ov003_02364928->currentSlot].row) {
    x = 128 - (Ov003_GetTextWidth(Text_Get(0x7d000023)) >> 1);
    for (i = 0; i < strlen(Text_Get(0x7d000023)); i++) {
      x += Ov003_DrawLetterAlt(((const u8*)Text_Get(0x7d000023))[i], x & 0x1ff,
                               80);
    }
    brightness = Math_Ease(0, 0, 16, obj->frame, 48);
  } else {
    brightness = Math_Ease(0, 0, -16, obj->frame, 48);
  }
  savedGfx = data_027c0280;
  savedScreen = data_027c027c;
  Screen_SetCurrentBoth(data_027c0288.unk_00);
  data_027c0280->masterBrightness = brightness;
  Screen_SetCurrentBoth(data_027c0288.unk_04);
  data_027c0280->masterBrightness = brightness;
  Screen_SetCurrent(savedScreen);
  Screen_SetCurrentGfx(savedGfx);
  if (obj->frame == 48) {
    obj->frame = 0;
    Task_ResumeScript(obj);
  }
}
