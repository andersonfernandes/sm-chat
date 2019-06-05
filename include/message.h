#include <iostream>
#include <ctime>

#ifndef MESSAGE_H
#define MESSAGE_H

#define BROADCAST 'B'
#define UNICAST 'U'

typedef struct {
  char source_name[50];
  char text[200];
  char mode;
  time_t sent_at;
} Message;

#endif
