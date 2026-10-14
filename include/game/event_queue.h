#ifndef GAME_EVENT_QUEUE_H
#define GAME_EVENT_QUEUE_H

#include "types.h"

#define EVENT_QUEUE_SIZE 16
/* Events EventQueue_Drain hands on at a time, as (id, arg) byte pairs. */
#define EVENT_BATCH_SIZE 4

/* Link messages of the map select menu. */
#define EVENT_MAP_SELECT_PREV_LIST 1
#define EVENT_MAP_SELECT_NEXT_LIST 2
#define EVENT_MAP_SELECT_PAGE 3     /* arg: page to turn to */
#define EVENT_MAP_SELECT_POSITION 4 /* arg: position on the page */
#define EVENT_MAP_SELECT_CONFIRM 5  /* arg: position on the page */
#define EVENT_MAP_SELECT_LEAVE 6
#define EVENT_MAP_SELECT_PICK_FIRST_MAP 7 /* arg: position on the page */
#define EVENT_MAP_SELECT_CANCEL_FIRST_MAP 8

/* Link messages of the Vs setup team screen. The team ones add the
 * player slot (0-3) to the id. */
#define EVENT_VS_SETUP_CONTROLLER \
  1                         /* to 8: controller entry id - 1; arg: controller */
#define EVENT_VS_SETUP_CO 9 /* to 16: CO entry id - 9; arg: CO */
#define EVENT_VS_SETUP_READY 17
#define EVENT_VS_SETUP_CANCEL \
  18 /* cancel on the Vs setup player and CO buttons, a guess */
#define EVENT_VS_TEAM_PREV 19 /* up key: previous team */
#define EVENT_VS_TEAM_NEXT 23 /* down key: next team */
#define EVENT_VS_TEAM_READY 27
#define EVENT_VS_TEAM_BACK 28 /* leave the ready state, or the screen */

/* Link messages of the Vs setup settings menu, sent by the host. */
#define EVENT_VS_SETTING_SELECT_ROW 29 /* arg: setting row */
#define EVENT_VS_SETTING_INCREASE 30
#define EVENT_VS_SETTING_DECREASE 31
#define EVENT_VS_SETTINGS_CONFIRM 32
#define EVENT_VS_SETTINGS_CANCEL 33

typedef struct EventQueueEntry {
  u8 id;
  u8 arg;
} EventQueueEntry;

/* Ring buffer of (id, arg) events. The queue is empty when readIndex == writeIndex. */
typedef struct EventQueue {
  EventQueueEntry entries[EVENT_QUEUE_SIZE];
  u8 readIndex;
  u8 writeIndex;
} EventQueue;

extern EventQueue gEventQueue;

/* Adds (id, arg); drops it when the queue is full. */
void EventQueue_Push(int id, int arg);

/* Empties the queue. */
void EventQueue_Reset(void);

/* Takes up to 4 events off the queue and hands them, padded with id 0,
 * to func_0207b4b0. */
void EventQueue_Drain(void);

#endif
