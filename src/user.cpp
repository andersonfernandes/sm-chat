#include <iostream>
#include <sys/shm.h>
#include "../include/user.h"

key_t users_key = (key_t) rand();
key_t users_count_key = (key_t) rand();

User* att_users(int users_shmid) {
  return (User*) shmat(users_shmid, NULL, 0);
}

int* att_users_count(int users_count_shmid) {
  return (int*) shmat(users_count_shmid, NULL, 0);
}
