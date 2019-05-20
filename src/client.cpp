#include <thread>
#include <iostream> 
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdio.h> 
#include <string.h>

#include "user.h"

using namespace std; 

int users_shmid = shmget(users_key, MAX_USERS * sizeof(user_t), 0666);

int main() {
  if (users_shmid < 0) {
    perror("shmget"); 
		exit(1);
	}

  user_t *users = (user_t*) shmat(users_shmid, NULL, 0);

  user_t *current_user = new user_t;

  cout << "Username: ";
  cin >> current_user->name;

  memcpy(&users[0], current_user, sizeof(user_t));
  
	shmdt(users);

  while(1) {}
	return 0;
}
