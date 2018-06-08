#include "stm32f30x_conf.h"
#include "30010_io.h"
#include "hardware_control.h"
#include "draw_objects.h"
#include "vectors.h"

int main(void) {
    //variables
    uint8_t x1 = 1, y1 = 1, x2 = 100, y2 = 30; // window size
    x2 = (((x2 - x1 - 1) / 10) * 10) + x1 + 1; // makes the width divisible by 10 (number of boxes in game)

    //initialisation
    init_usb_uart(115200);
    initJoyStick();
    initTimer();



    while(1) {

    }
}
