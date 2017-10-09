#include <stdio.h>
#include "mbed.h"
#include "mbed_events.h"
#include "rtos.h"

EventQueue _event_queue_one(32 * EVENTS_EVENT_SIZE, NULL);
EventQueue _event_queue_two(32 * EVENTS_EVENT_SIZE, NULL);

Thread _can_thread;
Thread _sensor_thread;

void event_queue_task() { printf("Event Task executed... \r\n"); }

void event_queue_task_irq() {
  printf("Event added to queue... \r\n");
  _event_queue_one.call(&event_queue_task);
}

void event_queue_sensor_task() { printf("Event Sensor Task executed... \r\n"); }

void event_queue_sensor_task_irq() {
  printf("Sensor Event added to queue... \r\n");
  _event_queue_two.call(&event_queue_sensor_task);
}

void can_task() {
  printf("CAN Task... \r\n");
  while (1) {
    printf("Event dispatched... \r\n");
    _event_queue_one.dispatch(100);
    Thread::wait(100);
  }
}
void sensor_task() {
  printf("Sensor Task... \r\n");
  while (1) {
    printf("Sensor Event dispatched... \r\n");
    _event_queue_two.dispatch(100);
    Thread::wait(100);
  }
}

int main() {
  osStatus can_status = _can_thread.start(can_task);
  if (can_status != osOK) {
    printf("[INFO] CAN thread not initialized \r\n");
  } else {
    printf("[INFO] CAN thread initialized ... [OK]\r\n");
  }

  osStatus sensor_status = _sensor_thread.start(sensor_task);
  if (sensor_status != osOK) {
    printf("[INFO] Sensor thread not initialized \r\n");
  } else {
    printf("[INFO] Sensor thread initialized ... [OK]\r\n");
  }

  Ticker canTicker;
  canTicker.attach(&event_queue_task_irq, 2.0f);

  Ticker sensorTicker;
  sensorTicker.attach(&event_queue_sensor_task_irq, 10.0f);

  while (1) {
    printf("In main loop. \r\n");
    Thread::wait(100);
  }
}
