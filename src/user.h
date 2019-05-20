#include <iostream> 
#include <sys/ipc.h> 

using namespace std;

#define USERS_BASEKEY "users_basekey"
#define MAX_USERS 25

typedef struct {
  int id;
  int shmid;
  string name;
  bool valid;
} user_t;

key_t users_key = ftok(USERS_BASEKEY, 65);
