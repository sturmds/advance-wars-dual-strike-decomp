#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_0201a774.h"

extern Unk_02080440* data_027c027c;
extern u16 gEventChoiceAnswer;

/* Choice bits from ChoicePrompt_GetKeys (keys) and ChoicePrompt_PollTouch (touch), as in
 * EventChoice_Update. */
#define CHOICE_KEY_LEFT 0x04
#define CHOICE_KEY_RIGHT 0x08
#define CHOICE_KEY_CONFIRM 0x10

int ChoicePrompt_GetKeys(void);
u16 ChoicePrompt_PollTouch(void);
void DialogueCursor_Animate(void);
void func_0201d730(void* text);
void ScriptSlot_ClearCallback(int slot);
void ChoicePrompt_DrawCursor(Unk_0201a774* prompt);
void Sound_PlaySe(int se);

/* Update of the two-choice prompt in the event text window. Right moves the
 * cursor from choice 0 to 1, left back. A move redraws the cursor and plays
 * a sound. Confirm stores the cursor in gEventChoiceAnswer, clears the text,
 * redraws and lets the waiting script of the current slot go on. */
void ChoicePrompt_Update(Unk_0201a774* prompt) {
  u16 held = ChoicePrompt_GetKeys();
  u32 keys = ChoicePrompt_PollTouch() | held;

  DialogueCursor_Animate();
  if (prompt->cursor != 0) {
    if (prompt->cursor == 1) {
      if (keys & CHOICE_KEY_LEFT) {
        prompt->cursor = 0;
        ChoicePrompt_DrawCursor(prompt);
        Sound_PlaySe(0x10002);
        return;
      }
    }
  } else {
    if (keys & CHOICE_KEY_RIGHT) {
      prompt->cursor = 1;
      ChoicePrompt_DrawCursor(prompt);
      Sound_PlaySe(0x10002);
      return;
    }
  }
  if (keys & CHOICE_KEY_CONFIRM) {
    gEventChoiceAnswer = prompt->cursor;
    func_0201d730(prompt->text);
    prompt->redraw();
    ScriptSlot_ClearCallback(*data_027c027c->scriptSlotCurrent & 0xff);
    Sound_PlaySe(0x1000a);
  }
}
