

/**
 * main.c
 */

#include "cyBot_uart.h"
#include "lcd.h"
#include "cyBot_Scan.h"
#include "open_interface.h"
#include "movement.h"

int main(void)
{
    cyBot_uart_init();
    cyBOT_init_Scan();
    cyBOT_Scan_t scan;
    lcd_init();
    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);

    int i;

    while(1) {
    char letter = cyBot_getByte();

    lcd_printf("Got a %c", letter);

    if(letter == 'w') {
        oi_setWheels(500, 500);
    }

    else if(letter == 's') {
        oi_setWheels(-200, -200);
    }

    else if(letter == 'a') {
        oi_setWheels(300, -300);
    }

    else if(letter == 'd') {
        oi_setWheels(-300, 300);
    }

    else if(letter == 32) {
        oi_setWheels(0,0);
    }

    else if(letter == 'm') {
        for (i = 0; i < 180; i += 5) {
            cyBOT_Scan(i, &scan);
            lcd_printf("Distance: %f Angle: %d", scan.sound_dist, i);
        }
        //cyBot_sendByte()

    }
    }

    oi_free(sensor_data);
}
