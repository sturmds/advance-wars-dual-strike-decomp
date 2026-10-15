#include "types.h"
#include "game/Unk_02080440.h"
#include "game/link_reaction_task.h"
#include "game/task.h"

u8* LinkReaction_GetText(int slot, int button);

extern TaskScriptCmd data_0216050c[];
extern u8 data_0216056c
    []; /* the drawable characters: A-Z, a-z, ". ", 0-9, "!?,:/&-'" */

/* Starts a task that scrolls a player's reaction text across the screen.
 * Glyph line: 0, then 1 + the sender's army colour, then one glyph per
 * character found in data_0216056c (its index + 7; others are skipped),
 * then 1. */
void LinkReaction_Show(int slot, int button) {
  u8* text = LinkReaction_GetText(slot, button);
  LinkReactionTask* task =
      (LinkReactionTask*)Task_Create(data_0216050c, TASK_LIST(3));
  u8* c;

  task->slot = slot;
  task->button = button;
  task->glyphCount = 0;
  task->glyphs[task->glyphCount] = 0;
  task->glyphCount++;
  task->glyphs[task->glyphCount] =
      data_027c027c->unk_48[data_027c0284]->players[slot].armyColour + 1;
  task->glyphCount++;
  for (; *text != 0; text++) {
    for (c = data_0216056c; *c != 0; c++) {
      if (*text == *c) {
        task->glyphs[task->glyphCount] = c - data_0216056c + 7;
        task->glyphCount++;
        break;
      }
    }
  }
  task->glyphs[task->glyphCount] = 1;
  task->glyphCount++;
}
