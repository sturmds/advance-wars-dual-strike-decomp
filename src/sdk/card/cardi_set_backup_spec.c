#include "sdk/card.h"

void MI_CpuFill8(void *dst, u32 value, u32 size);

void func_02115858(int type)
{
    CARDiCommandArg *cmd = data_022abb00.cmd;
    u32 size;

    MI_CpuFill8(&cmd->spec, 0, sizeof(CARDiBackupSpec));
    cmd->type = type;
    if (type == 0) {
        return;
    }
    size = 1 << ((type >> 8) & 0xff);
    cmd->spec.totalSize = size;
    if ((type & 0xff) == SAVE_CHIP_TYPE_EEPROM) {
        switch (size) {
        case 0x200:
            cmd->spec.pageSize = 16;
            cmd->spec.addrWidth = 1;
            cmd->spec.unk_10 = 0x4f;
            return;
        case 0x2000:
            cmd->spec.pageSize = 32;
            cmd->spec.addrWidth = 2;
            cmd->spec.unk_10 = 0x4f;
            return;
        case 0x10000:
            cmd->spec.pageSize = 128;
            cmd->spec.addrWidth = 2;
            cmd->spec.unk_10 = 0x9e;
            return;
        }
    } else if ((type & 0xff) == SAVE_CHIP_TYPE_FLASH) {
        switch (size) {
        case 0x40000:
            cmd->spec.unk_14 = 0x18b;
            cmd->spec.unk_18 = 0x127f;
            cmd->spec.unk_24 = 0x127f;
            cmd->spec.unk_20 = 0x13435;
            cmd->spec.unk_1C = 0x13435;
            break;
        case 0x80000:
        case 0x100000:
            cmd->spec.unk_14 = 0x18b;
            cmd->spec.unk_18 = 0;
            cmd->spec.unk_24 = 0x127f;
            cmd->spec.unk_20 = 0x13435;
            break;
        default:
            goto unsupported;
        }
        cmd->spec.sectorSize = 0x10000;
        cmd->spec.pageSize = 0x100;
        cmd->spec.addrWidth = 3;
        cmd->spec.unk_10 = 0x4f;
        return;
    } else if ((type & 0xff) == SAVE_CHIP_TYPE_FRAM) {
        switch (size) {
        case 0x2000:
        case 0x8000:
            cmd->spec.pageSize = size;
            cmd->spec.addrWidth = 2;
            return;
        }
    }
unsupported:
    cmd->type = 0;
    cmd->spec.totalSize = 0;
    data_022abb00.cmd->result = CARD_RESULT_UNSUPPORTED;
}
