#include <iostream> 
#include <ctime>

#include "user.h"

using namespace std;

typedef struct {
  user_t source;
  string message;
  bool readed;
  time_t send_at;
} message_t;

