#include <iostream> 
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdio.h> 

using namespace std; 

int main() {
  cout << "Starting Server" << endl;
  cout << "Looking for new clients" << endl; 

  key_t key = ftok("shmfile", 65);

  int shmid = shmget(key,1024, 0666|IPC_CREAT); 
  int *sm = (int*) shmat(shmid, NULL, 0);

  cout << sm[0] << endl;

  shmdt(sm); 
  shmctl(shmid, IPC_RMID, NULL);

  return 0;
}
