#include "sdk/snd.h"
#include "sdk/snd_seq.h"

SndTrack *func_arm7_03800ea4(SndPlayer *player, int trackIndex);
void SndTrack_ReleaseChannels(SndTrack *track, SndPlayer *player, int release);
void SndTrack_FreeChannels(SndTrack *track);
void func_arm7_037feebc(void);
void SND_StartIntervalTimer(void);
BOOL func_arm7_037fffc8(SndPlayer *player, BOOL doPeriodic);
void func_arm7_03800df4(SndPlayer *player);

/* Moves the player's sequence ticks ticks forward without playing it: releases
 * and frees every channel first, then steps with the interval timer stopped. */
void SndArm7_SkipSeq(int playerNo, u32 ticks)
{
    SndPlayer *player = &data_arm7_038085d0[playerNo];
    int i;
    u32 tick;

    for (i = 0; i < SND_TRACK_COUNT; i++) {
        SndTrack *track = func_arm7_03800ea4(player, i);
        if (track != NULL) {
            SndTrack_ReleaseChannels(track, player, 127);
            SndTrack_FreeChannels(track);
        }
    }
    func_arm7_037feebc();
    for (tick = 0; tick < ticks; tick++) {
        if (func_arm7_037fffc8(player, FALSE)) {
            func_arm7_03800df4(player);
            break;
        }
    }
    SND_StartIntervalTimer();
    if (data_arm7_0380808c != NULL) {
        data_arm7_0380808c->player[player->number].tickCounter += tick;
    }
}
