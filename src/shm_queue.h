#include "message.h"

#ifndef SHM_QUEUE_H

#define SHM_QUEUE_H
#define MAX_MESSAGES 10000

typedef struct {
  int capacity;
  int size;
  int front;
  int rear;
  int messages_list_shmid;
} ShmQueue;

ShmQueue* create_queue();
bool empty(ShmQueue* shmq);
bool full(ShmQueue* shmq);
void enqueue(ShmQueue* shmq, Message* message);
void dequeue(ShmQueue* shmq, Message* dest_message);

ShmQueue* att_shmq(int shmq_id);

#endif
