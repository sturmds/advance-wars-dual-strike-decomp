#include "types.h"
#include "game/Unk_02080440.h"

/* Adds offset `index` of offset list `list` to x and y; FALSE at the end of the list. */
BOOL func_020c9a14(int *x, int *y, int list, int index);
void func_020c69c0(int x, int y, int arg);
void func_020c6b50(int x, int y, int player);
void func_020c4018(void);
void Power_ResetScrollAndRefresh(void);

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

/* Runs func_020c69c0 and func_020c6b50 for every tile of offset list 2 around (x, y). */
void func_020e462c(int x, int y)
{
    int pos[2];
    int index = 0;

    do {
        pos[0] = x;
        pos[1] = y;
        if (!func_020c9a14(&pos[0], &pos[1], 2, index++)) {
            break;
        }
        func_020c69c0(pos[0], pos[1], 30);
        func_020c6b50(pos[0], pos[1], data_027c027c->unk_48[data_027c0284]->currentPlayer);
    } while (1);
    func_020c4018();
    Power_ResetScrollAndRefresh();
}
