#include "types.h"

void Sound_PlaySe(int id);
void func_020e2670(int priority);

/* Sonja's CO Power routine. Its game effect is applied elsewhere: this
 * passes id 0xc0 to Sound_PlaySe (doc 890 calls it a sound) and starts
 * the task shared with other CO Powers. */
void SonjaFlash(int priority) {
  Sound_PlaySe(0xc0);
  func_020e2670(priority);
}
