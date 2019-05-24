#include <iostream> 
#include <sys/shm.h> 

using namespace std;

#define MAX_USERS 100

typedef struct {
  int shmid;
  char name[50];
} user_t;

key_t users_key = (key_t) 123123;
key_t users_count_key = (key_t) 234234;

user_t* att_users(int users_shmid) {
  return (user_t*) shmat(users_shmid, NULL, 0);
}

int* att_users_count(int users_count_shmid) {
  return (int*) shmat(users_count_shmid, NULL, 0);
}
