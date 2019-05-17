#include <thread>
#include <iostream> 
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdio.h> 

#include "user.h"

using namespace std; 

int main() {
	key_t users_key = ftok(USERS_FILE, 65);

	int users_shmid = -1;
	if ((users_shmid = shmget(users_key , 1024, 0666)) < 0) {
		perror("shmget"); 
		exit(1);
	}

	int *users = (int*) shmat(users_shmid, NULL, 0);

	shmdt(users);
	return 0;
}
