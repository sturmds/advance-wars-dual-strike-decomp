#include "sdk/user_settings.h"

void func_arm7_027d4f3c(u32 offset, u32 size, void *dest);    /* read firmware flash */
void func_arm7_037fde5c(u32 data, void *dest, int size);       /* fill words */
void func_arm7_037fde70(const void *src, void *dest, int size); /* copy words */
void func_arm7_037fdf68(const void *src, void *dest, u32 size); /* copy bytes */
u16 func_arm7_03807514(u16 value);
int UserSettings_ChooseCopy(UserSettings *copies);

/*
 * Reads the two user settings copies from firmware flash, blanks the unused
 * tail of the chosen copy's nickname and comment, and copies it to the
 * shared area at 0x027ffc80 (zeros if neither copy is good). Then puts the
 * MAC address and the channel word from flash right after it.
 */
void UserSettings_LoadFromFlash(void)
{
    UserSettings *shared;
    SharedWirelessInfo *wireless;
    u32 offset;
    int chosen;
    u16 channels;
    UserSettings copies[2];
    u8 mac[6];
    volatile u32 zero;  /* needed to match: the original stores and reloads the 0 */
    int i;

    shared = SHARED_USER_SETTINGS;
    func_arm7_027d4f3c(FLASH_USER_SETTINGS_OFFSET, 2, &offset);
    offset <<= 3;
    func_arm7_027d4f3c(offset, sizeof(UserSettings), &copies[0]);
    func_arm7_027d4f3c(offset + 0x100, sizeof(UserSettings), &copies[1]);

    /* needed to match: `(copies - 1)[chosen]` and `p + i - 1` let the
       compiler fold the - 1s into the offsets, as the original does */
    chosen = UserSettings_ChooseCopy(copies);
    if (chosen != 0) {
        if ((copies - 1)[chosen].nicknameLength < USER_SETTINGS_NICKNAME_MAX) {
            for (i = USER_SETTINGS_NICKNAME_MAX; i > (copies - 1)[chosen].nicknameLength; i--) {
                *((copies - 1)[chosen].nickname + i - 1) = 0;
            }
        }
        if ((copies - 1)[chosen].commentLength < USER_SETTINGS_COMMENT_MAX) {
            for (i = USER_SETTINGS_COMMENT_MAX; i > (copies - 1)[chosen].commentLength; i--) {
                *((copies - 1)[chosen].comment + i - 1) = 0;
            }
        }
        func_arm7_037fde70(&copies[chosen - 1], shared, sizeof(UserSettings));
    } else {
        zero = 0;
        func_arm7_037fde5c(zero, shared, sizeof(UserSettings));
    }

    func_arm7_027d4f3c(FLASH_MAC_ADDRESS, 6, mac);
    wireless = (SharedWirelessInfo *)(shared + 1);
    func_arm7_037fdf68(mac, wireless->macAddress, 6);
    func_arm7_027d4f3c(FLASH_CHANNELS, 2, &channels);
    wireless->channels = func_arm7_03807514(channels >> 1);
}
