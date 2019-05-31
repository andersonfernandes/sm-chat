#include <iostream>
#include <ctime>
#include "user.h"

#ifndef MESSAGE_H
#define MESSAGE_H

typedef struct {
  char source[50];
  char text[200];
  time_t sent_at;
} Message;

#endif
