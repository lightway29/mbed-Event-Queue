#ifndef COMMON_H
#define COMMON_H

#include "mbed.h"
#include "mbed_events.h"
#include "rtos.h"

class Common {
 public:
  Common();

 private:
  EventQueue _can_event_queue;
};
#endif
