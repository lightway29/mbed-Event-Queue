#include <stdio.h>
#include "mbed.h"
#include "mbed_events.h"
#include "rtos.h"

EventQueue _event_queue_one(32 * EVENTS_EVENT_SIZE, NULL);
EventQueue _event_queue_two(32 * EVENTS_EVENT_SIZE, NULL);
Thread _can_thread;

volatile bool sensor_interrupt_flag = 0;

void event_queue_task() { printf("Event Task executed... \r\n"); }

void event_queue_sensor_task() { printf("Event Sensor Task executed... \r\n"); }

void event_queue_task_irq() {
  printf("Event added to queue... \r\n");
  _event_queue_one.call(&event_queue_task);
}

void event_queue_sensor_task_irq() {
  printf("Sensor Event added to queue... \r\n");
  sensor_interrupt_flag = 1;
  _event_queue_two.call(&event_queue_sensor_task);
}

void task_manager() {
  while (1) {
    if (sensor_interrupt_flag) {
      _event_queue_one.break_dispatch();
      _event_queue_two.dispatch(100);
      sensor_interrupt_flag = 0;
    } else {
      _event_queue_one.dispatch(100);
    }

    Thread::wait(100);
  }
}

int main() {
  osStatus can_status = _can_thread.start(task_manager);
  if (can_status != osOK) {
    printf("CAN thread not initialized \r\n");
  } else {
    printf("CAN thread initialized ... [OK]\r\n");
  }

  Ticker canTicker;
  canTicker.attach(&event_queue_task_irq, 3.0f);

  while (1) {
    event_queue_task_irq();
    Thread::wait(1000);
  }
}
