#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iomanip>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "user.h"
#include "message.h"

using namespace std;

int users_shmid = shmget(users_key, MAX_USERS * sizeof(user_t), 0666);
int users_count_shmid = shmget(users_count_key, sizeof(int), 0666);

int current_user_index = -1;
user_t current_user;
char menu_option;

void init();
void create_user();
void print_users_list();

int main() {
  init();
  create_user();
  
  user_t *users = att_users(users_shmid);
  current_user = users[current_user_index];
  shmdt(users);

  system("clear");
  cout << "\n >  Welcome " << current_user.name << " \n" <<  endl;

  cout << "1 - Send messages" << endl;
  cout << "2 - Receive messages" << endl;

  cout << "\n> Select one option to continue: ";
  cin >> menu_option;

  if(menu_option == '1') {
    print_users_list();
  } else if (menu_option == '2') {

  } else {  
    cout << "OTHER";
  }

  while(1) {  }
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

void print_users_list() {
  user_t *users = att_users(users_shmid);
  int *users_count = att_users_count(users_count_shmid);

  if(*users_count == 1) {
    cout << "0 Users online!" << endl;
  } else {
    users = att_users(users_shmid);

    cout << "\nOnline users:" << endl;
    for (int i = 0; i < (*users_count); i++) {
      if (i == current_user_index) continue;
      cout << i << " - " << users[i].name << endl;
    }

    shmdt(users);
  }

  shmdt(users_count);
}
