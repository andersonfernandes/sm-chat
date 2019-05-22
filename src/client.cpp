#include <thread> 
#include <iostream> 
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdio.h> 
#include <signal.h> 
#include <stdlib.h>
#include <time.h>

#include "user.h"

using namespace std; 

int users_shmid = shmget(users_key, MAX_USERS * sizeof(user_t), 0666);
int users_count_shmid = shmget(users_count_key, sizeof(int), 0666); 

void create_user();

int main() {
  srand(time(NULL));
  if (users_shmid < 0) {
    perror("shmget"); 
    exit(1);
  }

  create_user();

  while(1) {}
  return 0;
}

void create_user() {
  user_t *users = att_users(users_shmid);
  int *users_count = att_users_count(users_count_shmid);

  user_t *current_user = new user_t();

  cout << "Username: ";
  cin >> current_user->name;
  current_user->shmid = rand();

  memcpy(&users[(*users_count)], current_user, sizeof(user_t));

  int new_count = (*users_count) + 1;
  memcpy(users_count, &new_count, sizeof(int));

  shmdt(users);
  shmdt(users_count);
}
