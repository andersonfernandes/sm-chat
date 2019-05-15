#include <thread>
#include <vector>
#include <iostream> 
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdio.h> 

using namespace std; 

void clients_sign_in(int id) {
  cout << "Looking for new clients" << endl; 

  key_t key = ftok("shmfile", 65); 

  int shmid = shmget(key,1024,0666|IPC_CREAT); 

  vector<string> *sm = (vector<string>*) shmat(shmid, NULL, 0);

  string sss = (*sm).front();

  cout << sss;

  shmdt(sm); 

  shmctl(shmid,IPC_RMID,NULL); 
}

int main() {
  cout << "Starting Server" << endl;

  thread clients_sign_in_thread(clients_sign_in, 1);
  clients_sign_in_thread.join();

  return 0;
}
