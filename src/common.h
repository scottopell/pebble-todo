#ifndef COMMON_H
#define COMMON_H
#include <pebble.h>

#define MAX_ISSUES 8
typedef struct{
  uint64_t timestamp;
  char title[30];
} issue_t;

// global functions
void app_msg_init( void );

#endif
