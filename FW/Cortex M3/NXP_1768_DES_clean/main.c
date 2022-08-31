#include "uart_arduino.h"
#include "pinata_protocol.h"
#include "trigger_arduino.h"
int main(void)
{
	uart_arduino_init0();
	trigger_setup();
    while(1)
    {
    	pinata_comannd_loop();
    }
}
