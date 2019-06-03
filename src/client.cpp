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

int current_user_index = -1;
User current_user;
User user_to_chat;
char chat_mode;

void init();
void create_user();
void print_users_list();
void receive_messages();
void send_messages();
void print_message(Message* message);

int main() {
  int user_to_chat_index = -1;
  init();
  create_user();

  User *users = att_users(users_shmid);
  current_user = users[current_user_index];
  shmdt(users);

  system("clear");
  cout << "\n>  Welcome " << current_user.name << " \n" <<  endl;

  cout << "1 - Unicast Mode" << endl;
  cout << "2 - Broadcast Mode" << endl;

  cout << endl << "Select a chat mode to continue" << endl;
  cout << "> ";
  cin >> chat_mode;
  cin.clear();
  cout << endl;

  if(chat_mode == '1') {
    print_users_list();
    cout << endl << "Select a user to chat" << endl;
    cout << "> ";
    cin >> user_to_chat_index;
  }

  thread receive_messages_thread(receive_messages);
  receive_messages_thread.detach();

  send_messages();

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
  User *users = att_users(users_shmid);
  int *users_count = att_users_count(users_count_shmid);

  User *current_user = new User();

  cout << "Username: ";
  cin.getline(current_user->name, 50);
  current_user->key = (key_t) rand();

  memcpy(&users[(*users_count)], current_user, sizeof(User));

  current_user_index = *users_count;
  int new_count = (*users_count) + 1;
  memcpy(users_count, &new_count, sizeof(int));

  shmdt(users);
  shmdt(users_count);
}

void print_users_list() {
  User *users = att_users(users_shmid);
  int *users_count = att_users_count(users_count_shmid);

  if(*users_count == 1) {
    cout << endl << "0 Users online!" << endl;
  } else {
    users = att_users(users_shmid);

    cout << endl << "Online users:" << endl;
    for (int i = 0; i < (*users_count); i++) {
      if (i == current_user_index) continue;
      cout << i << " - " << users[i].name << endl;
    }

    shmdt(users);
  }

  shmdt(users_count);
}

void receive_messages() {
  while(true) {
    ShmQueue* shmq = att_shmq(current_user.messages_shmid);
    if(!empty(shmq)) {
      Message* message = new Message();
      dequeue(shmq, message);
      print_message(message);
    }
    shmdt(shmq);
  }
}

void send_messages() {
  char text[200];
  do {
    cin.getline(text, 200);

    Message* message = new Message();
    strcpy(message->source, current_user.name);
    strcpy(message->text, text);
    message->sent_at = time(0);

    if(chat_mode == '1' && strlen(message->text) != 0) {
      // TODO Implement
    } else if(chat_mode == '2' && strlen(message->text) != 0) {
      User *users = att_users(users_shmid);
      int *users_count = att_users_count(users_count_shmid);

      for (int i = 0; i < *users_count; i++) {
        ShmQueue* shmq = att_shmq(users[i].messages_shmid);
        enqueue(shmq, message);
        shmdt(shmq);
      }

      shmdt(users);
      shmdt(users_count);
    }
  } while(strcmp(text, ":q") != 0);

  exit(0);
}

void print_message(Message* message) {
  cout << endl << ctime(&message->sent_at);
  cout << " > " << message->source << ": " << message->text << endl << endl;
}
