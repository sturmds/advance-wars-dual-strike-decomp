#include "types.h"
#include "sdk/card_arm7.h"
#include "text_data.h"

/* The handlers take the card state; some are still written with a
 * placeholder argument type in their own files. */
void func_arm7_038037e8(CARDiArm7Common* common);
void func_arm7_038037fc(CARDiArm7Common* common);
void func_arm7_03803818(CARDiArm7Common* common);
void func_arm7_03803834(CARDiArm7Common* common);
void func_arm7_03803850(CARDiArm7Common* common);
void func_arm7_0380386c(CARDiArm7Common* common);
void func_arm7_0380388c(CARDiArm7Common* common);
void func_arm7_038038a8(CARDiArm7Common* common);
void func_arm7_038038ac(CARDiArm7Common* common);
void func_arm7_038038b0(CARDiArm7Common* common);
void func_arm7_038038b4(CARDiArm7Common* common);

/* The two command bytes come from the ROM at build time
 * (config/AWRE/data_inc.txt). mwcc emits these definitions smallest first,
 * and these two bytes in reverse source order, which puts the write enable
 * byte first, as in the ROM. */
#pragma section TEXT_DATA begin

/* Save chip command 5, read status (CardSave_IsChipIdle). */
u8 gCardSaveCmdReadStatus = {
#include "data/arm7/gCardSaveCmdReadStatus.inc.c"
};

/* Save chip command 6, write enable (CardSave_SendWriteEnable). */
u8 gCardSaveCmdWriteEnable = {
#include "data/arm7/gCardSaveCmdWriteEnable.inc.c"
};

/* Request handlers, indexed by CARDiArm7Common.command
 * (Card_ServiceThread_Arm7). */
void (*gCardRequestHandlers[13])(CARDiArm7Common* common) = {
    func_arm7_038038b0, func_arm7_038038ac, func_arm7_038038a8,
    func_arm7_0380388c, func_arm7_0380386c, func_arm7_038038b4,
    func_arm7_03803850, func_arm7_03803818, func_arm7_03803834,
    func_arm7_038037fc, func_arm7_038038b4, func_arm7_038037e8,
    func_arm7_038038b4,
};

#pragma section TEXT_DATA end
