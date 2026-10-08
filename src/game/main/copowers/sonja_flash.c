#include "types.h"

void func_0200b76c(int id);
void func_020e2670(int priority);

/* Sonja's CO Power routine. Its game effect is applied elsewhere: this
 * passes id 0xc0 to func_0200b76c (doc 890 calls it a sound) and starts
 * the task shared with other CO Powers. */
void SonjaFlash(int priority)
{
    func_0200b76c(0xc0);
    func_020e2670(priority);
}
