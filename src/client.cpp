#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <thread>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "user.h"
#include "message.h"
#include "shm_queue.h"

using namespace std;

int users_shmid = shmget(users_key, MAX_USERS * sizeof(User), 0666);
int users_count_shmid = shmget(users_count_key, sizeof(int), 0666);

User current_user;
char chat_mode;

void init();
void create_user();
void print_users_list();
void receive_messages();
void send_messages();
void print_message(Message* message);

int main() {
  char menu_option = '1';
  bool invalid_option = false;

  init();
  create_user();

  do {
    system("clear");
    cout << endl << ">  Welcome " << current_user.name <<  endl;

    if(invalid_option) cout << endl << "Invalid Option!" << endl;

    cout << endl << "1 - Start Chat" << endl;
    cout << "0 - Exit" << endl;

    cout << endl << "Select a option to continue" << endl;
    cout << "> ";
    cin >> menu_option;

    if(menu_option == '1') {
      invalid_option = false;
      cout << endl << "Started chat. Type :q to go back to the menu" << endl << endl;

      thread receive_messages_thread(receive_messages);
      receive_messages_thread.detach();

      send_messages();
    } else {
      invalid_option = true;
    }
  } while(menu_option != '0');

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

int create_shmq() {
  key_t key = rand();
  int shmq_id = shmget(key, sizeof(ShmQueue), 0666|IPC_CREAT);

  ShmQueue* shmq = att_shmq(shmq_id);
  memcpy(shmq, create_queue(), sizeof(ShmQueue));
  shmdt(shmq);

  return shmq_id;
}

void create_user() {
  User *users = att_users(users_shmid);
  int *users_count = att_users_count(users_count_shmid);

  User *new_user = new User();

  cout << "Username: ";
  cin.getline(new_user->name, 50);
  new_user->shmq_id = create_shmq();

  memcpy(&users[(*users_count)], new_user, sizeof(User));
  memcpy(users_count, &++(*users_count), sizeof(int));
  memcpy(&current_user, new_user, sizeof(User));

  shmdt(users);
  shmdt(users_count);
}

void print_users_list() {
  // User *users = att_users(users_shmid);
  // int *users_count = att_users_count(users_count_shmid);
  //
  // if(*users_count == 1) {
  //   cout << endl << "0 Users online!" << endl;
  // } else {
  //   users = att_users(users_shmid);
  //
  //   cout << endl << "Online users:" << endl;
  //   for (int i = 0; i < (*users_count); i++) {
  //     if (i == current_user_index) continue;
  //     cout << i << " - " << users[i].name << endl;
  //   }
  //
  //   shmdt(users);
  // }
  //
  // shmdt(users_count);
}

void receive_messages() {
  while(true) {
    ShmQueue* shmq = att_shmq(current_user.shmq_id);
    if(!empty(shmq)) {
      Message* message = new Message();

      if(dequeue(shmq, message)) print_message(message);
    }
    shmdt(shmq);
  }
}

void send_messages() {
  while(true) {
    char text[200];
    cin.getline(text, 200);

    Message* message = new Message();
    strcpy(message->source, current_user.name);
    strcpy(message->text, text);
    message->sent_at = time(0);

    if(strcmp(text, ":q") == 0) break;
    else if(strlen(message->text) != 0) {
      User *users = att_users(users_shmid);
      int *users_count = att_users_count(users_count_shmid);

      for (int i = 0; i < *users_count; i++) {
        ShmQueue* shmq = att_shmq(users[i].shmq_id);
        enqueue(shmq, message);
        shmdt(shmq);
      }

      shmdt(users);
      shmdt(users_count);
    }
  }
}

void print_message(Message* message) {
  cout << endl << ctime(&message->sent_at);
  cout << " > " << message->source << ": " << message->text << endl << endl;
}
