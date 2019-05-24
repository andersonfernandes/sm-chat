#include <iostream> 
#include <string.h>
#include <signal.h> 
#include <sys/ipc.h> 
#include <sys/shm.h> 

#include "user.h"

using namespace std; 

bool kill_server_flag = false;
int users_shmid = shmget(users_key, MAX_USERS * sizeof(user_t), 0666|IPC_CREAT); 
int users_count_shmid = shmget(users_count_key, sizeof(int), 0666|IPC_CREAT); 

int old_count = 0;

void init();
void sigint_handler(int sig_num);
void print_new_users();

int main() {
  init();

  while (1) {
    print_new_users();

    if(kill_server_flag) {
      cout << "\nShutting down the server!" << endl;

      shmctl(users_shmid, IPC_RMID, NULL);
      shmctl(users_count_shmid, IPC_RMID, NULL);
      exit(0);
    }
  }

  return 0;
}

void init() {
  cout << "Starting the server" << endl;

  int *users_count = att_users_count(users_count_shmid);
  *users_count = 0;
  shmdt(users_count);

  cout << "Use Ctrl-C to stop\n" << endl;
  signal(SIGINT, sigint_handler); 
}

void sigint_handler(int sig_num) { 
  kill_server_flag = true;
} 

void print_new_users() {
  user_t *users = att_users(users_shmid);
  int *users_count = att_users_count(users_count_shmid);

  if(old_count != *users_count) {
    old_count = (*users_count);

    time_t now = time(0);
    cout << ctime(&now) << " > ";
    cout << users[(*users_count) - 1].name << " logged on the server\n" <<  endl;
  }

  shmdt(users_count);
  shmdt(users);
}
