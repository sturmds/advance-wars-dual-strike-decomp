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

extern EventQueue data_0229d89c;

#endif
