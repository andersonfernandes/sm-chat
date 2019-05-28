#include "messages_queue.h"

typedef struct messages_queue MessagesQueue;
struct messages_queue  {
  int front;
  int rear;
  int size;
  message_t* messages[MAX_MESSAGES];
};

MessagesQueue* create_queue() {
  MessagesQueue* mq = new MessagesQueue(); 
  mq->front = mq->size = 0;  
  mq->rear = MAX_MESSAGES - 1;
  return mq;
}

bool full(MessagesQueue* mq) {
  return mq->size == MAX_MESSAGES;
}

bool empty(MessagesQueue* mq) {
  return mq->size == 0;
}

void enqueue(MessagesQueue* mq, message_t* message) {
  if(full(mq)) return;  

  mq->rear = (mq->rear + 1) % MAX_MESSAGES;  
  mq->messages[mq->rear] = message;  
  mq->size = mq->size + 1;  
}

message_t* dequeue(MessagesQueue* mq) {
  if(empty(mq)) return NULL;  

  message_t* message = mq->messages[mq->front];  
  mq->front = (mq->front + 1) % MAX_MESSAGES;  
  mq->size = mq->size - 1;  
  return message; 
}

message_t* front(MessagesQueue* mq) {
  if(empty(mq)) return NULL;
  return mq->messages[mq->front];
}

message_t* rear(MessagesQueue* mq) {  
    if (empty(mq)) return NULL;  
    return mq->messages[mq->rear];  
}  
