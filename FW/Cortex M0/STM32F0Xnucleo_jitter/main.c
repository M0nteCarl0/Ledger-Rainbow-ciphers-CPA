
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "hal.h"
#include "pinata_protocol.h"

int main(void)
{
	init_uart();
	trigger_setup();
	led_onboard_init();
	pinata_comannd_init();
	led_onboard_on();
    while (1) 
    {   
		pinata_comannd_loop();
	
    }
}

