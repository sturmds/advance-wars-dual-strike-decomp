#include "types.h"
#include "sdk/snd.h"

/* Entry in one of the system's callback lists. next is filled in when the
 * entry is added. */
typedef struct CallbackNode {
  void (*func)(void);
  void* arg;
  struct CallbackNode* next;
} CallbackNode;

static s8 data_022a8020;
static u32 data_022a8028; /* set once the sound system is set up */
static u32 data_022a8024;
static CallbackNode data_022a8038;
static CallbackNode data_022a802c;

void func_02104de8(void);
void func_02104ddc(void);
void func_021148ec(CallbackNode* node);
void func_021148d4(CallbackNode* node);
void* func_02104f00(void);
void func_021062a4(void);
void SoundSeqPlayer_InitAll(void);

/* Sets up the sound system once: registers its two callbacks, clears the
 * alarm masks and the sequence players. */
void SoundSystem_Init(void) {
  if (data_022a8028 != 0) {
    return;
  }
  data_022a8028 = 1;
  SND_Init();
  data_022a802c.func = func_02104de8;
  data_022a802c.arg = NULL;
  data_022a8038.func = func_02104ddc;
  data_022a8038.arg = NULL;
  func_021148ec(&data_022a802c);
  func_021148d4(&data_022a8038);
  func_02104f00();
  func_021062a4();
  SoundSeqPlayer_InitAll();
  data_022a8020 = -1;
  data_022a8024 = 1;
}
