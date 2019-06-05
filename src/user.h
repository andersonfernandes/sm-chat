#include <iostream>
#include <sys/shm.h>

#ifndef USER_H

#define USER_H
#define MAX_USERS 100

typedef struct {
  int shmq_id;
  char name[50];
} User;

extern key_t users_key;
extern key_t users_count_key;

User* att_users(int users_shmid);
int* att_users_count(int users_count_shmid);

#endif
