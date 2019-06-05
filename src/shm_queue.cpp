#include <iostream>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "../include/shm_queue.h"

key_t generate_key();

ShmQueue* create_queue() {
  ShmQueue* shmq = new ShmQueue();
  shmq->capacity = MAX_MESSAGES;  
  shmq->front = shmq->size = 0;  
  shmq->rear = MAX_MESSAGES - 1;
  shmq->locked = false;
  shmq->messages_list_shmid = shmget(generate_key(), MAX_MESSAGES * sizeof(Message), 0666|IPC_CREAT);

  return shmq;
}

bool empty(ShmQueue* shmq) {
  return (shmq->size == 0);
}

bool full(ShmQueue* shmq) {
  return (shmq->size == shmq->capacity);
}

void enqueue(ShmQueue* shmq, Message* message) {
  if(full(shmq)) return;
  shmq->locked = true;

  shmq->rear = (shmq->rear + 1) % shmq->capacity;
  shmq->size = shmq->size + 1;

  Message* messages = (Message*) shmat(shmq->messages_list_shmid, NULL, 0);
  memcpy(&messages[shmq->rear], message, sizeof(Message));
  shmdt(messages);
  shmq->locked = false;
}

bool dequeue(ShmQueue* shmq, Message* dest_message) {
  if(empty(shmq) || shmq->locked) return false;

  Message* messages = (Message*) shmat(shmq->messages_list_shmid, NULL, 0);
  memcpy(dest_message, &messages[shmq->front], sizeof(Message));
  shmdt(messages);

  shmq->front = (shmq->front + 1) % shmq->capacity;  
  shmq->size = shmq->size - 1;

  return true;
}

ShmQueue* att_shmq(int shmid) {
  return (ShmQueue*) shmat(shmid, NULL, 0);
}

key_t generate_key() {
  srand(time(NULL));
  return (key_t) rand();
}
