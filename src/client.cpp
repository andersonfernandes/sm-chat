#include <thread>
#include <iostream> 
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdio.h> 

using namespace std; 

int main() {
  key_t key = ftok("shmfile", 65); 

  int shmid = shmget(key, 1024, 0666);

  if(shmid > 0) {
    int *sm = (int*) shmat(shmid, NULL, 0);

    sm[0] = 45;

    shmdt(sm);
  }

  return 0;
}
