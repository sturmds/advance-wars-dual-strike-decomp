#include "types.h"

void Sound_PlaySe(int id);
void func_020e2670(int priority);

/* Hachi's CO Power routine. Its game effect is applied elsewhere: this
 * passes id 0xde to Sound_PlaySe (doc 890 calls it a sound) and starts
 * the task shared with other CO Powers. */
void HachiFlash(int priority) {
  Sound_PlaySe(0xde);
  func_020e2670(priority);
}
