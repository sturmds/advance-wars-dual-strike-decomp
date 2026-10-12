#include "types.h"
#include "game/task.h"

typedef struct Unk_gMenuState Unk_gMenuState;

struct Unk_gMenuState {
  u8 pad_00[0x2eb];
  u8 unk_2eb;
};

extern Unk_gMenuState gMenuState;
extern const TaskScriptCmd data_0215d038[];

void func_020647f8(Task* parent) {
  gMenuState.unk_2eb = 1;
  Task_CreateChild(data_0215d038, parent);
}
