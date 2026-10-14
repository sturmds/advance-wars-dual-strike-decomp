#include "types.h"
#include "game/rect.h"
#include "game/task.h"
#include "game/event_script.h"

extern u16 gEventChoiceAnswer;
extern Rect gEventChoiceButtons[];

/* EventChoice_ReadKeys bits */
#define CHOICE_KEY_LEFT 0x04
#define CHOICE_KEY_RIGHT 0x08
#define CHOICE_KEY_CONFIRM 0x10 /* A */
#define CHOICE_KEY_CANCEL 0x20  /* B */

#define CHOICE_LAST 3 /* also where B puts the cursor */

u16 EventChoice_ReadKeys(void);
u16 Touch_TestButtonRects(const Rect* rects, int count);
void DialogueCursor_Animate(void);
void EventChoice_DrawRecord(int choice, int x, int y);
void EventChoice_DrawCursor(Task* task);
void func_0201d730(void* text);
void Task_ResumeScript(Task* task);
void Sound_PlaySe(int se);

/* The cursor sits in Task.waitFrames. Needed to match: it is read as a
 * signed halfword. */
#define CURSOR(task) (*(s16*)&(task)->waitFrames)

/* Update of an event script prompt with four choices side by side. Left and
 * right move the cursor, B jumps to the last choice, touch buttons 0-3 pick
 * one. A cursor move redraws the choice text at tile (7, 3). A or touch
 * button 4 confirms: gEventChoiceAnswer gets 2, 3, 4 for choices 0-2 and 1 for the
 * last one, then the task passes unk_38 + 0x4e to func_0201d730, calls
 * unk_3c and resumes its script. */
void EventChoice_Update(Task* task) {
  u16 keys;
  u16 touched;
  int oldCursor;

  keys = EventChoice_ReadKeys();
  touched = Touch_TestButtonRects(gEventChoiceButtons, EVENT_CHOICE_COUNT);
  DialogueCursor_Animate();
  oldCursor = CURSOR(task);
  if (keys & CHOICE_KEY_CANCEL) {
    CURSOR(task) = CHOICE_LAST;
  }
  if (keys & CHOICE_KEY_LEFT) {
    if (CURSOR(task) > 0) {
      CURSOR(task)--;
    }
  }
  if (keys & CHOICE_KEY_RIGHT) {
    if (CURSOR(task) < CHOICE_LAST) {
      CURSOR(task)++;
    }
  }
  if (touched & 1) {
    CURSOR(task) = 0;
  }
  if (touched & 2) {
    CURSOR(task) = 1;
  }
  if (touched & 4) {
    CURSOR(task) = 2;
  }
  if (touched & 8) {
    CURSOR(task) = 3;
  }
  if (oldCursor != CURSOR(task)) {
    EventChoice_DrawRecord(CURSOR(task), 7, 3);
    EventChoice_DrawCursor(task);
    Sound_PlaySe(0x10002);
  }
  if ((keys & CHOICE_KEY_CONFIRM) || (touched & 0x10)) {
    switch (CURSOR(task)) {
      case 0:
        gEventChoiceAnswer = 2;
        break;
      case 1:
        gEventChoiceAnswer = 3;
        break;
      case 2:
        gEventChoiceAnswer = 4;
        break;
      case CHOICE_LAST:
      default:
        gEventChoiceAnswer = 1;
        break;
    }
    func_0201d730((u8*)task->unk_38 + 0x4e);
    task->unk_3c();
    Task_ResumeScript(task);
    Sound_PlaySe(0x1000a);
  }
}
