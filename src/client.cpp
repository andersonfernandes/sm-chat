#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <thread>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "../include/user.h"
#include "../include/message.h"
#include "../include/shm_queue.h"

using namespace std;

int users_shmid = shmget(users_key, MAX_USERS * sizeof(User), 0666);
int users_count_shmid = shmget(users_count_key, sizeof(int), 0666);

User current_user;

void init();
void create_user();
void select_user();
void receive_messages();
void send_messages();
void print_message(Message* message);

int main() {
  init();
  create_user();

  system("clear");
  cout << endl << "> Welcome " << current_user.name <<  endl;
  cout << endl << "The chat started. To quit the app type :q" << endl << endl;

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

void select_user(User* user_to_chat) {
  User *users = att_users(users_shmid);
  int *users_count = att_users_count(users_count_shmid);

  if(*users_count == 1) {
    cout << endl << "0 Users online!" << endl;
  } else {
    users = att_users(users_shmid);

    cout << endl << "Online users:" << endl;
    for (int i = 0; i < (*users_count); i++) {
      if (users[i].shmq_id == current_user.shmq_id) continue;
      cout << i << " - " << users[i].name << endl;
    }

    int user_to_chat_index;
    cout << endl << "Select the user to chat: ";
    cin >> user_to_chat_index;

    memcpy(user_to_chat, &users[user_to_chat_index], sizeof(User));
  }

  shmdt(users);
  shmdt(users_count);
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
    strcpy(message->source_name, current_user.name);
    strcpy(message->text, text);

    if(strcmp(text, ":q") == 0) break;
    else if(strlen(message->text) != 0) {
      char message_mode = 'B';
      cout << endl << "Select the message mode ([U]nicast or [B]roadcast): ";
      cin >> message_mode;

      if(message_mode == 'U' || message_mode == 'u') {
        User* user_to_chat = new User();
        select_user(user_to_chat);

        message->mode = UNICAST;
        message->sent_at = time(0);

        ShmQueue* shmq = att_shmq(user_to_chat->shmq_id);
        enqueue(shmq, message);
        shmdt(shmq);
      } else {
        message->mode = BROADCAST;
        message->sent_at = time(0);

        User *users = att_users(users_shmid);
        int *users_count = att_users_count(users_count_shmid);

        for (int i = 0; i < *users_count; i++) {
          if (users[i].shmq_id == current_user.shmq_id) continue;

          ShmQueue* shmq = att_shmq(users[i].shmq_id);
          enqueue(shmq, message);
          shmdt(shmq);
        }

        shmdt(users);
        shmdt(users_count);
      }
    }

    cout << endl;
  }
}

void print_message(Message* message) {
  cout << endl << ctime(&message->sent_at);
  cout << " > " << message->source_name << " (" << message->mode << "): " << message->text << endl << endl;
}
