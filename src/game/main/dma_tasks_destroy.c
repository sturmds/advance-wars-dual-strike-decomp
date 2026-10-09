#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

void Task_DestroyByScript(const void *descriptor);

extern Unk_02080440 *data_027c027c;
extern Unk_027c0288 data_027c0288;
extern u8 data_0216e054[];       /* DMA task state, see func_0200371c */
extern u8 data_021571ac[];       /* script of the first DMA task */
extern u8 data_021571cc[];       /* script of the second DMA task */

/* Destroys the tasks of the DMA pair that func_0200371c
 * started: both when data_0216e054[2] is set, else the one that belongs to
 * the current screen. */
void DmaTasks_Destroy(void)
{
    if (data_0216e054[2] != 0) {
        Task_DestroyByScript(data_021571ac);
        Task_DestroyByScript(data_021571cc);
        return;
    }
    if ((void *)data_027c027c == data_027c0288.unk_00) {
        if (data_0216e054[0] == 1) {
            Task_DestroyByScript(data_021571ac);
        } else {
            Task_DestroyByScript(data_021571cc);
        }
    } else {
        if (data_0216e054[0] == 2) {
            Task_DestroyByScript(data_021571ac);
        } else {
            Task_DestroyByScript(data_021571cc);
        }
    }
}
