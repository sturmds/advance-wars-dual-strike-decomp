#include "sdk/fs.h"

/* Moves a file to the end of the list that starts at head. head is a
 * list node (the archive's list pointers), not a file. */
void FSi_AppendToList(FSFile *file, FSFile *head)
{
    FSFile *prev = file->prev;
    FSFile *next = file->next;

    if (prev != NULL) {
        prev->next = next;
    }
    if (next != NULL) {
        next->prev = prev;
    }

    next = head->next;
    while (next != NULL) {
        head = next;
        next = next->next;
    }
    head->next = file;
    file->prev = head;
    file->next = NULL;
}
