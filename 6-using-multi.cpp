#include <bcm2835.h>
#include <iostream>

using namespace std;

const int size = 6;
const int half = size / 2;
int pin[size] = {0, 5, 6, 13, 19, 26};

int main() {
    if (!bcm2835_init()) {
        return 1;
    }

    for (int c = 0; c != size; c++) {
        bcm2835_gpio_fsel(pin[c], BCM2835_GPIO_FSEL_OUTP);
    }

    int p = 0;
    while(1) {
        int mask = (1 << pin[p]) + (1 << pin[p + half]);
                
        bcm2835_gpio_write_multi(mask, 1);
        bcm2835_delay(200);
        bcm2835_gpio_write_multi(mask, 0);

        p = (p + 1) % half;
    }

    return 0;
}
