#include <stdio.h>
#include "mbed_events.h"
#include "rtos.h"

#define ZHARK_CAN_EVENT_SIZE 32
#define ZHARK_CAN_STACK_SIZE 1024

EventQueue _can_callback_queue(32 * EVENTS_EVENT_SIZE, NULL);

Thread* _can_thread;

void can_bus_task() {
  printf("Event queue dispached.. \r\n");
  // _can_callback_queue.dispatch_forever();

  while (1) {
    _can_callback_queue.dispatch();
    Thread::wait(10);
  }
}

void can_task() { printf("Event queue task.. \r\n"); }

int main() {
  // _can_thread = new Thread(osPriorityNormal, ZHARK_CAN_STACK_SIZE, NULL, "CAN thread");
  // osStatus status = _can_thread->start(callback(&_can_callback_queue, &EventQueue::dispatch_forever));

  // normal priority thread for other events
  // Thread eventThread(osPriorityNormal);
  // _can_callback_queue.start(callback(&_can_callback_queue, &EventQueue::dispatch_forever));
  //
  // // call blink_led2 every second, automatically defering to the eventThread
  // Ticker ledTicker;
  // ledTicker.attach(_can_callback_queue.event(&can_task), 1.0f);

  //
  //

  // _can_callback_queue.dispatch();
  while (1) {
    _can_callback_queue.dispatch(10);
    printf("Event added \r\n");
    Thread::wait(100);
  }

  //
  // while (1) {
  //   wait(0.1);
  //   _can_callback_queue.event(&can_task);
  // }
}
