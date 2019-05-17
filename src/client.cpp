#include <thread>
#include <iostream> 
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdio.h> 

using namespace std; 

#define CLIENTS_FILE "clients_file"

int main() {
	key_t clients_key = ftok(CLIENTS_FILE, 65);

	int clients_shmid = -1;
	if ((clients_shmid = shmget(clients_key , 1024, 0666)) < 0) {
		perror("shmget"); 
		exit(1);
	}

	int *clients = (int*) shmat(clients_shmid, NULL, 0);

	shmdt(clients);
	return 0;
}
