#include "types.h"
#include "game/Unk_02080440.h"
#include "game/script_slot.h"

extern Unk_02080440* data_027c027c;
void ScriptSlot_CmdWait(void);
void ScriptSlot_CmdSetCallback(void);
void ScriptSlot_CmdCallFunction(void);
void ScriptSlot_CmdJump(void);
void func_02016eb8(void);
void ScriptSlot_CmdWaitForScript(void);
void ScriptSlot_CmdStartScript(void);
void func_020170f0(void);
void ScriptSlot_CmdSetOnStop(void);
void func_02016e1c(void);
void ScriptSlot_CmdWaitForEventScripts(void);
void ScriptSlot_CmdStopSlot(void);
void func_02016d24(void);
void func_02016cd0(void);
void ScriptSlot_CmdMovePath(void);
void ScriptSlot_CmdMoveAccel(void);
void ScriptSlot_CmdMoveArc(void);
void func_02015f50(void);
void func_02015efc(void);
void ScriptSlot_CmdMoveX(void);
void ScriptSlot_CmdMoveY(void);
void ScriptSlot_CmdWaitUntilOne(void);
void ScriptSlot_CmdWaitWhile(void);
void func_02016bb4(void);
void func_02016b60(void);
void func_02016b0c(void);
void func_02016ab8(void);
void ScriptSlot_CmdJumpIfFlag(void);
void func_020168fc(void);
void func_020168a8(void);
void func_02016950(void);
void func_020169a4(void);
void ScriptSlot_CmdSetFlag(void);

static ScriptCommandFunc gScriptCommands[] = {
    (ScriptCommandFunc)&ScriptSlot_CmdWait,
    (ScriptCommandFunc)&ScriptSlot_CmdSetCallback,
    (ScriptCommandFunc)&ScriptSlot_CmdCallFunction,
    (ScriptCommandFunc)&ScriptSlot_CmdJump,
    (ScriptCommandFunc)&func_02016eb8,
    (ScriptCommandFunc)&ScriptSlot_CmdWaitForScript,
    (ScriptCommandFunc)&ScriptSlot_CmdStartScript,
    (ScriptCommandFunc)&func_020170f0,
    (ScriptCommandFunc)&func_020170f0,
    (ScriptCommandFunc)&ScriptSlot_CmdWait,
    (ScriptCommandFunc)&ScriptSlot_CmdSetOnStop,
    (ScriptCommandFunc)&func_02016e1c,
    (ScriptCommandFunc)&ScriptSlot_CmdWaitForEventScripts,
    (ScriptCommandFunc)&ScriptSlot_CmdStopSlot,
    (ScriptCommandFunc)&func_02016d24,
    (ScriptCommandFunc)&func_02016cd0,
    (ScriptCommandFunc)&ScriptSlot_CmdMovePath,
    (ScriptCommandFunc)&ScriptSlot_CmdMoveAccel,
    (ScriptCommandFunc)&ScriptSlot_CmdMoveArc,
    (ScriptCommandFunc)&func_02015f50,
    (ScriptCommandFunc)&func_02015efc,
    (ScriptCommandFunc)&ScriptSlot_CmdMoveX,
    (ScriptCommandFunc)&ScriptSlot_CmdMoveY,
    (ScriptCommandFunc)&ScriptSlot_CmdWaitUntilOne,
    (ScriptCommandFunc)&ScriptSlot_CmdWaitWhile,
    (ScriptCommandFunc)&func_02016bb4,
    (ScriptCommandFunc)&func_02016b60,
    (ScriptCommandFunc)&func_02016b0c,
    (ScriptCommandFunc)&func_02016ab8,
    (ScriptCommandFunc)&ScriptSlot_CmdJumpIfFlag,
    (ScriptCommandFunc)&func_020168fc,
    (ScriptCommandFunc)&func_020168a8,
    (ScriptCommandFunc)&func_02016950,
    (ScriptCommandFunc)&func_020169a4,
    (ScriptCommandFunc)&ScriptSlot_CmdSetFlag};

/* Counts down the slot's wait timer; once it reaches 0 and no callback is
 * pending, runs commands until a handler returns 0. */
void ScriptSlot_Run(int slot) {
  if (data_027c027c->scriptSlots[slot].start == NULL) {
    return;
  }
  if (data_027c027c->scriptSlots[slot].wait != 0) {
    data_027c027c->scriptSlots[slot].wait--;
    if (data_027c027c->scriptSlots[slot].wait != 0) {
      return;
    }
  }
  if (data_027c027c->scriptSlots[slot].callback != NULL) {
    return;
  }
  if (data_027c027c->scriptSlots[slot].start == NULL) {
    return;
  }
  if (data_027c027c->scriptSlots[slot].pc == NULL) {
    return;
  }
  while (gScriptCommands[data_027c027c->scriptSlots[slot].pc->opcode](slot)) {
  }
}
