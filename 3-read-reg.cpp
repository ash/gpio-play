#include <bcm2835.h>
#include <iostream>
#include <bitset>

using namespace std;

const int size = 28;
int pin[size] = {2, 3, 4, 17, 27, 22, 10, 9, 11, 0, 5, 6, 13, 19, 26,
              14, 15, 18, 23, 24, 25, 8, 7, 1, 12, 16, 20, 21};

int main() {
    if (!bcm2835_init()) {
        return 1;
    }

    for (int c = 0; c != size; c++) {
        bcm2835_gpio_fsel(pin[c], BCM2835_GPIO_FSEL_INPT);
        bcm2835_gpio_set_pud(pin[c], BCM2835_GPIO_PUD_UP);
    }

    uint32_t* gpioBASE = bcm2835_regbase(BCM2835_REGBASE_GPIO);

    while(1) {
        uint32_t reg1 = bcm2835_peri_read(gpioBASE + BCM2835_GPLEV0 / 4);
       // uint32_t reg2 = bcm2835_peri_read(gpioBASE + BCM2835_GPLEV1 / 4);

        cout << bitset<32>(reg1) << endl;

        bcm2835_delay(100);
    }

    return 0;
}
