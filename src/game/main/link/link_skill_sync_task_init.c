#include "types.h"
#include "game/co_data.h"
#include "game/skills.h"
#include "game/Unk_02080440.h"
#include "game/link_sync_task.h"
#include "game/player.h"

void* Mem_Alloc(u32 size);
int Co_GetForceRankSetIndex(int coId, int player);
void Skills_Copy(u32* src, u8* dst);

/* Fills the 128 bytes the skill sync sends: for each of the 4 players and their 2 COs, the
 * 16 bytes of skills that CO has equipped in the Force Rank set the player uses. */
void LinkSkillSyncTask_Init(LinkSyncTask* task) {
  int co;
  int player;
  int slot;

  task->skills = Mem_Alloc(sizeof(data_0229de6c));
  for (player = 1; player <= PLAYER_SLOT_LAST; player++) {
    for (slot = 0; slot < PLAYER_CO_COUNT; slot++) {
      co = MAP_STATE->playerCos[player][slot] & 0x7f;
      if (co != 0) {
        Skills_Copy(gCoEquippedSkills[co][Co_GetForceRankSetIndex(co, player)],
                    task->skills + ((player - 1) * 2 + slot) * 16);
      }
    }
  }
  task->sentBytes = 0;
  task->receivedBytes = 0;
  task->sendDone = 0;
  task->started = 0;
  task->receiveStarted = 0;
}
