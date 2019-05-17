#include <thread> 
#include <iostream> 
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdio.h> 

using namespace std; 

#define CLIENTS_FILE "clients_file"

void kill_server_listener() {
  string exit_server = "";
  do {
    cout << "type exit to stop the server\n" << endl;
    cin >> exit_server;
  } while(exit_server.compare("exit"));
}

int main() {
  cout << "Starting Server" << endl;

  key_t clients_key = ftok(CLIENTS_FILE, 65);

  int clients_shmid = shmget(clients_key, 1024, 0666|IPC_CREAT); 
  int *clients = (int*) shmat(clients_shmid, NULL, 0);

  thread kill_server_listener_thread(kill_server_listener);
  kill_server_listener_thread.join();

  shmdt(clients); 
  shmctl(clients_shmid, IPC_RMID, NULL);

  return 0;
}
