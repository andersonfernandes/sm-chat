#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "user.h"

using namespace std;

int users_shmid = shmget(users_key, MAX_USERS * sizeof(user_t), 0666);
int users_count_shmid = shmget(users_count_key, sizeof(int), 0666);

int current_user_index = -1;

void init();
void create_user();

int main() {
  init();
  create_user();

  while(1) {}
  return 0;
}

void init() {
  srand(time(NULL));

  if ((users_shmid < 0) || (users_count_shmid < 0)) {
    perror("shmget");
    cout << "Unable to connect with the server!" << endl;

    exit(1);
  }
}

void create_user() {
  user_t *users = att_users(users_shmid);
  int *users_count = att_users_count(users_count_shmid);

  user_t *current_user = new user_t();

  cout << "Username: ";
  cin.getline(current_user->name, 50);
  current_user->key = (key_t) rand();

  memcpy(&users[(*users_count)], current_user, sizeof(user_t));

  current_user_index = *users_count;
  int new_count = (*users_count) + 1;
  memcpy(users_count, &new_count, sizeof(int));

  shmdt(users);
  shmdt(users_count);
}
