#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "user.h"
#include "message.h"
#include "shm_queue.h"

using namespace std;

bool kill_server_flag = false;
int users_shmid = shmget(users_key, MAX_USERS * sizeof(User), 0666|IPC_CREAT);
int users_count_shmid = shmget(users_count_key, sizeof(int), 0666|IPC_CREAT);

int old_count = 0;

void init();
void sigint_handler(int sig_num);
void process_new_users();

int main() {
  init();

  while (1) {
    process_new_users();

    if(kill_server_flag) {
      cout << "\nCleaning shared memory segments" << endl;

      User *users = att_users(users_shmid);
      int *users_count = att_users_count(users_count_shmid);
      for (int i = 0; i < (*users_count); i++) {

        ShmQueue* shmq = (ShmQueue*) shmat(users[i].shmq_id, NULL, 0);
        shmctl(shmq->messages_list_shmid, IPC_RMID, NULL);
        shmdt(shmq);

        shmctl(users[i].shmq_id, IPC_RMID, NULL);
      }

      shmdt(users_count);
      shmdt(users);

      shmctl(users_shmid, IPC_RMID, NULL);
      shmctl(users_count_shmid, IPC_RMID, NULL);

      cout << "\nBye!" << endl;
      exit(0);
    }
  }

  return 0;
}

void init() {
  system("clear");
  cout << "\n> Starting the server \n" <<  endl;

  int *users_count = att_users_count(users_count_shmid);
  *users_count = 0;
  shmdt(users_count);

  cout << "Use Ctrl-C to stop\n" << endl;
  signal(SIGINT, sigint_handler);

}
void sigint_handler(int sig_num) {
  kill_server_flag = true;
}

void process_new_users() {
  User *users = att_users(users_shmid);
  int *users_count = att_users_count(users_count_shmid);

  if(old_count != *users_count) {
    for (int i = old_count; i < *users_count; i++) {
      User user = users[i];

      time_t now = time(0);
      cout << ctime(&now) << " > ";
      cout << user.name << " logged on the server\n" <<  endl;  
    }

    old_count = (*users_count);
  }

  shmdt(users_count);
  shmdt(users);
}
