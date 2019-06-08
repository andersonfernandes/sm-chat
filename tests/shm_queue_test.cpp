#define CATCH_CONFIG_MAIN
#include "catch.h"

#include <sys/ipc.h>
#include <sys/shm.h>
#include "../include/shm_queue.h"

void clear_setup(ShmQueue* shmq);
Message* provide_message();

SCENARIO("SHM Queue") {
  ShmQueue* shmq = create_queue();
  Message* message = provide_message();

  WHEN("#create_queue") {
    THEN("shmq pointer cant be NULL and the default attributes should be set") {
      CHECK(shmq != NULL);
      CHECK(shmq->capacity == MAX_MESSAGES);
      CHECK(shmq->size == 0);
      CHECK(shmq->front == 0);
      CHECK(shmq->messages_list_shmid != -1);
    }
  }

  WHEN("#empty") {
    CHECK(empty(shmq)); 
  }

  WHEN("#enqueue") {
    enqueue(shmq, message);

    CHECK(shmq->size == 1);
    CHECK(shmq->front == shmq->rear);
  }

  WHEN("#dequeue") {
    Message* dequeued_message = new Message();
    enqueue(shmq, message);

    CHECK(dequeue(shmq, dequeued_message));
    CHECK(shmq->size == 0);
    CHECK(shmq->front == 1);
    CHECK(shmq->rear == 0);
    CHECK(strcmp(dequeued_message->source_name, message->source_name) == 0);
    CHECK(strcmp(dequeued_message->text, message->text) == 0);
    CHECK(dequeued_message->mode == message->mode);
    CHECK(dequeued_message->sent_at == message->sent_at);
  }

  WHEN("#full") {
    for (int i = 0; i < MAX_MESSAGES; i++) {
      enqueue(shmq, provide_message());
    }

    CHECK(full(shmq)); 
  }

  clear_setup(shmq);
}

Message* provide_message() {
  Message* message = new Message();
  strcpy(message->source_name, "John Doe");
  strcpy(message->text, "Hello");
  message->mode = BROADCAST;
  message->sent_at = time(0);

  return message;
}

void clear_setup(ShmQueue* shmq) {
  shmctl(shmq->messages_list_shmid, IPC_RMID, NULL);
  free(shmq);
}
