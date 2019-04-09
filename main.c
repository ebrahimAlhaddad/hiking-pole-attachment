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
    char input = receive_rx();
    send_tx(input);
  }
}
