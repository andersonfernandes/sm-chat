#include <iostream>
#include <ctime>

#include "user.h"

using namespace std;

#ifndef MESSAGE_H

#define MESSAGE_H
#define MAX_MESSAGES 100000

typedef struct {
  user_t source;
  string message;
  bool readed;
  time_t send_at;
} message_t;

#endif
