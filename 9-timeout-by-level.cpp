#include <bcm2835.h>
#include <iostream>
#include <bitset>

using namespace std;

const int pin_in = 21;
const int pin_out = 14;
const int mask_in = 1 << pin_in;
const int mask_out = 1 << pin_out;

int main() {
    if (!bcm2835_init()) {
        return 1;
    }

    bcm2835_gpio_fsel(pin_in, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_set_pud(pin_in, BCM2835_GPIO_PUD_UP);
    
    bcm2835_gpio_fsel(pin_out, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_write_multi(mask_out, 0);

    uint32_t* gpioBASE = bcm2835_regbase(BCM2835_REGBASE_GPIO);

    // This is needed to prevent theinfluence of other experiments
    bcm2835_peri_write(gpioBASE + BCM2835_GPLEN0 / 4, 0);
    bcm2835_peri_write(gpioBASE + BCM2835_GPREN0 / 4, 0);
    bcm2835_peri_write(gpioBASE + BCM2835_GPFEN0 / 4, 0);
    bcm2835_peri_write(gpioBASE + BCM2835_GPAREN0 / 4, 0);
    bcm2835_peri_write(gpioBASE + BCM2835_GPAFEN0 / 4, 0);

    bcm2835_peri_write(gpioBASE + BCM2835_GPLEN0 / 4, mask_in);

    int c = 0;
    while(1) {
        int x = bcm2835_peri_read(gpioBASE + BCM2835_GPEDS0 / 4);
        cout << bitset<32>(x) << endl;

        if (x) {
            bcm2835_gpio_write_multi(mask_out, 1);
            c++;
        }
        
        if (c == 5) {
            bcm2835_peri_write(gpioBASE + BCM2835_GPEDS0 / 4, mask_in);
            bcm2835_gpio_write_multi(mask_out, 0);
            c = 0;
        }
        bcm2835_delay(200);
    }

    return 0;
}
