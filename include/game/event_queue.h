#ifndef GAME_EVENT_QUEUE_H
#define GAME_EVENT_QUEUE_H

#include "types.h"

#define EVENT_QUEUE_SIZE 16

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
