#include "types.h"
#include "game/Unk_ov005_0235a30c.h"

Ov005Task* Ov005Slide_StartEntryA(Unk_ov005_0235a30c* work, int entryIndex);
Ov005Task* Ov005Slide_StartEntryB(Unk_ov005_0235a30c* work, int entryIndex);

/* Creates the task of each entry of both tables. Needed to match: both loops share
 * one counter. */
void Ov005Slide_StartAll(Unk_ov005_0235a30c* work) {
  int i = 0;

  for (;;) {
    if (work->entriesA[i].id == 0) {
      break;
    }
    work->entriesA[i].task = Ov005Slide_StartEntryA(work, i);
    i++;
  }
  i = 0;
  for (;;) {
    if (work->entriesB[i].id == 0) {
      return;
    }
    work->entriesB[i].task = Ov005Slide_StartEntryB(work, i);
    i++;
  }
}
