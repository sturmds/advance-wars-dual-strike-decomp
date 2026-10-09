#include "types.h"
#include "game/Unk_02080440.h"

void func_020c88fc(void);
void func_020f79a4(void);

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

/* Two optional callbacks, reached through the pointer at offset 0x50 of the
 * array that starts at data_027c027c->unk_48 and is indexed by data_027c0284. */
typedef struct Unk_02080440_50 {
    void (*first)(void);
    void (*second)(void);
} Unk_02080440_50;

#define HOOKS (((Unk_02080440_50 **)&data_027c027c->unk_48[2])[data_027c0284])

/* Runs func_020c88fc (it derives view values from the map data), then the
 * two callbacks if set. Many menus and Map_PlacePredeployedUnits call it;
 * the purpose is not clear yet. */
void func_020c8888(void)
{
    func_020c88fc();
    if (HOOKS->first != NULL) {
        HOOKS->first();
    }
    if (HOOKS->second != NULL) {
        HOOKS->second();
    }
    func_020f79a4();
}
