#include "main.h"

void init_ports() {

}

void init_memory() {

}

int main(void) {
  init_ports();

  init_memory();
  serial_init();

  while (1) {
    sprintln("Hello World");
    _delay_ms(1000);
  }
}
