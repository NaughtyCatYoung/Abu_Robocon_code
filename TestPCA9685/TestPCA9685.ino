/* Libs */
#include <PCA9685_LED_DRIVER.h>

/* Defines */
#define ADDRESS     0x40
#define FREQUENCY   400     //min 24Hz, max 1524Hz

/* Class Constructor */
PCA9685 gpio = (ADDRESS);

void setup()
{
    gpio.begin(FREQUENCY);
}

void loop()
{
    for (uint8_t channel = 0; channel < 16; channel++) gpio.setPWM(channel, 0xFFF);
    gpio.update();
    delay(2000);
    for (uint8_t channel = 0; channel < 16; channel++) gpio.setPWM(channel, 0x000);
    gpio.update();
    delay(2000);
}