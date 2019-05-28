#include <iostream>
#include <sys/shm.h>

#ifndef USER_H

#define USER_H
#define MAX_USERS 100

typedef struct {
  key_t key;
  int messages_shmid;
  char name[50];
} User;

key_t users_key = (key_t) 123123;
key_t users_count_key = (key_t) 234234;

User* att_users(int users_shmid) {
  return (User*) shmat(users_shmid, NULL, 0);
}

int* att_users_count(int users_count_shmid) {
  return (int*) shmat(users_count_shmid, NULL, 0);
}

#endif
