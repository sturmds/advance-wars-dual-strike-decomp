#include "types.h"

void func_0200b76c(int id);
void func_020e2670(int priority);

/* Hachi's CO Power routine. Its game effect is applied elsewhere: this
 * passes id 0xde to func_0200b76c (doc 890 calls it a sound) and starts
 * the task shared with other CO Powers. */
void HachiFlash(int priority)
{
    func_0200b76c(0xde);
    func_020e2670(priority);
}
