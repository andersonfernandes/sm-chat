#include <thread> 
#include <iostream> 
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdio.h> 

#include "user.h"

using namespace std; 

void kill_server_listener() {
  string exit_server = "";
  do {
    cout << "type exit to stop the server\n" << endl;
    cin >> exit_server;
  } while(exit_server.compare("exit"));
}

int main() {
  cout << "Starting Server" << endl;

  key_t users_key = ftok(USERS_FILE, 65);

  int users_shmid = shmget(users_key, 1024, 0666|IPC_CREAT); 
  int *users = (int*) shmat(users_shmid, NULL, 0);

  thread kill_server_listener_thread(kill_server_listener);
  kill_server_listener_thread.join();

  shmdt(users); 
  shmctl(users_shmid, IPC_RMID, NULL);
  return 0;
}
