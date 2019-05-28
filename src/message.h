#include <iostream>
#include <ctime>
#include "user.h"

#ifndef MESSAGE_H
#define MESSAGE_H

typedef struct {
  User source;
  char text[200];
  bool readed;
  time_t send_at;
} Message;

#endif
