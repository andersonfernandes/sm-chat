#include <iostream>
#include "messages_queue.h"

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

void enqueue(MessagesQueue* mq, Message* message) {
  if(full(mq)) return;  

  mq->rear = (mq->rear + 1) % MAX_MESSAGES;  
  mq->messages[mq->rear] = message;  
  mq->size = mq->size + 1;  
}

Message* dequeue(MessagesQueue* mq) {
  if(empty(mq)) return NULL;  

  Message* message = mq->messages[mq->front];  
  mq->front = (mq->front + 1) % MAX_MESSAGES;  
  mq->size = mq->size - 1;  
  return message; 
}

Message* front(MessagesQueue* mq) {
  if(empty(mq)) return NULL;
  return mq->messages[mq->front];
}

Message* rear(MessagesQueue* mq) {  
    if (empty(mq)) return NULL;  
    return mq->messages[mq->rear];  
}  
