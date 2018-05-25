#include <bcm2835.h>
#include <iostream>
#include <cstdlib>

using namespace std;

int main(int argc, char** argv) {
    if (!bcm2835_init()) {
        return 1;
    }

    if (argc < 2) {
        cerr << "Usage: ./a.out PIN1 [PIN2 [PIN3 ...]]\n";
        return 1;
    }

    int mask = 0;

    for (int c = 0; c < argc - 1; c++) {
        int n = atoi(argv[c + 1]);

        if (n < 0 || n > 31) {
            cerr << "Pin number must be between 0 and 31\n";
            return 1;
        }

        bcm2835_gpio_fsel(n, BCM2835_GPIO_FSEL_OUTP);
        mask |= 1 << n;
    }


    uint32_t* gpioBASE = bcm2835_regbase(BCM2835_REGBASE_GPIO);

    int p = 0;
    while(1) {
        bcm2835_peri_write(gpioBASE + BCM2835_GPSET0 / 4, mask);
        bcm2835_delay(500);
        bcm2835_peri_write(gpioBASE + BCM2835_GPCLR0 / 4, mask);
        bcm2835_delay(500);
    }

    return 0;
}
