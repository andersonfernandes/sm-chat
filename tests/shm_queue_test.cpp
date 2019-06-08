#define CATCH_CONFIG_MAIN
#include "catch.h"

#include "../include/shm_queue.h"

ShmQueue* shmq;

SCENARIO("SHM Queue") {
  WHEN("#create_queue") {
    shmq = create_queue();

    THEN("shmq pointer cant be NULL") {
      REQUIRE(shmq != NULL);
    }

    THEN("capacity should be MAX_MESSAGES") {
      REQUIRE(shmq->capacity == MAX_MESSAGES);
    }

    THEN("size and front should be 0") {
      REQUIRE(shmq->size == 0);
      REQUIRE(shmq->front == 0);
    }

    THEN("rear should be MAX_MESSAGES-1") {
      REQUIRE(shmq->rear == (MAX_MESSAGES-1));
    }

    THEN("locked should be false") {
      REQUIRE(shmq->locked == false);
    }

    THEN("messages_list_shmid should not be -1") {
      REQUIRE(shmq->messages_list_shmid != -1);
    }
  }
}
