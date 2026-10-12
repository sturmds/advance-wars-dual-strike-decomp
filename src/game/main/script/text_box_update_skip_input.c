#include "types.h"
#include "game/Unk_0201a774.h"

#define KEY_CONFIRM 0x10 /* A, or a touch in the text window */
#define KEY_SKIP 0x400   /* Start */

extern u8 gEventSkipState;
u32 func_02024a6c(void);
int func_02024ec0(void);
BOOL func_02014b28(void);
void func_0201d730(void* text);
void DialogueCursor_Animate(void);

/* Start asks to skip the event (gEventSkipState = 1) when func_02014b28 allows
 * it. Start, A or a touch in the text window then clears the text with
 * func_0201d730, calls redraw and clears unk_08. */
void TextBox_UpdateSkipInput(Unk_0201a774* prompt) {
  u16 keys = func_02024a6c() | func_02024ec0();

  if (keys & KEY_SKIP) {
    if (func_02014b28()) {
      gEventSkipState = 1;
    }
  }
  if (keys & (KEY_SKIP | KEY_CONFIRM)) {
    func_0201d730(prompt->text);
    prompt->redraw();
    prompt->unk_08 = NULL;
  }
  DialogueCursor_Animate();
}
