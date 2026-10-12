#include "types.h"

extern void* data_022a7e6c;
void Mem_Free(void* ptr);
void Sound_InitFromMemory(void);
void Sound_LoadResidentData(void);
void Sound_StartSong(int song);

void func_020fc79c(void) {
  if (data_022a7e6c != NULL) {
    Mem_Free(data_022a7e6c);
  }
  Sound_InitFromMemory();
  Sound_LoadResidentData();
  Sound_StartSong(7);
}
