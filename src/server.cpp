#include <thread> 
#include <iostream> 
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdio.h> 
#include <signal.h> 
#include <string.h>

#include "user.h"

using namespace std; 

bool kill_server_flag = false;
int users_shmid = shmget(users_key, MAX_USERS * sizeof(user_t), 0666|IPC_CREAT); 

void sigint_handler(int sig_num) { 
  kill_server_flag = true;
} 

void print_users() {
}

void initialize_users(user_t *users) {
  for (int i = 0; i < MAX_USERS; i++) {
    user_t *user = new user_t;

    user->id = i;
    user->shmid = -1;
    user->valid = false;
    user->name = "";

    memcpy(&users[i], user, sizeof(user_t));
  }
}

int main() {
  cout << "Starting the server" << endl;

  user_t *users = (user_t*) shmat(users_shmid, NULL, 0);
  initialize_users(users);

  shmdt(users);

  cout << "Use Ctrl-C to stop\n" << endl;
  signal(SIGINT, sigint_handler); 

  while (1) {
    print_users();

    if(kill_server_flag) {
      cout << "\nShutting down the server!" << endl;

      shmctl(users_shmid, IPC_RMID, NULL);
      exit(0);
    }
  }

  return 0;
}
