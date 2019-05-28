#include "message.h"

#ifndef MESSAGES_QUEUE_H

#define MESSAGES_QUEUE_H
#define MAX_MESSAGES 10000

typedef struct messages_queue MessagesQueue;

MessagesQueue* create_queue();
bool full(MessagesQueue* mq);
bool empty(MessagesQueue* mq);
void enqueue(MessagesQueue* mq, Message* message);
Message* dequeue(MessagesQueue* mq);
Message* front(MessagesQueue* mq);
Message* rear(MessagesQueue* mq);

#endif
