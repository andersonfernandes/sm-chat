#include <thread>
#include <vector>
#include <iostream> 
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdio.h> 

using namespace std; 

int main() {
  key_t key = ftok("shmfile", 65); 

  int shmid = shmget(key,1024,0666|IPC_CREAT); 

  vector<string> *sm = (vector<string>*) shmat(shmid, NULL, 0);
  (*sm).push_back("TEST");

  shmdt(sm); 

  return 0;
}
