#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "AHT20.h"

#define I2C_PORT i2c0

AHT20::AHT20() {
}

void AHT20::init(int sdaPort, int sclPort) {
    sleep_ms(20); // Wait 20ms for sensor to power on and go to idle state

    i2c_init(I2C_PORT, 400000);
    gpio_set_function(sdaPort, GPIO_FUNC_I2C);
    gpio_set_function(sclPort,GPIO_FUNC_I2C);
    gpio_pull_up(sdaPort);
    gpio_pull_up(sclPort);

    sleep_ms(40); // Wait 40ms after power on
    while(checkCalibration() == false) {
        sensorInit();
    }
}

void AHT20::sensorInit() {
    uint8_t reg[3] = {0xBE, 0x08, 0x00};

    i2c_write_blocking(I2C_PORT, addr, reg, 3, false);
    sleep_ms(10); // Wait 10ms for initialisation
}

bool AHT20::checkCalibration() {
    uint8_t statusReg = 0x71;
    uint8_t cal; // Result byte
    i2c_write_blocking(I2C_PORT, addr, &statusReg, 1, false);
    i2c_read_blocking(I2C_PORT, addr, &cal, 1, false);

    return ((cal >> 4) & 1) == 1; // Check 4th bit is equal to 1
}

void AHT20::triggerMeasurement() {
    while(checkCalibration() == false) {
        sensorInit();
    }

    uint8_t measureCommand[3] = {0xAC, 0x33, 0x00};
    uint8_t status; // result status
    uint8_t data[6]; // data array

    i2c_write_blocking(I2C_PORT, addr, measureCommand, 3, false);
    sleep_ms(80); // Wait for measurement

    i2c_read_blocking(I2C_PORT, addr, &status,1, false); // Read status byte

    // Wait for status to be ready before continuing
    while (((status >> 8) & 1) != 0) {
        printf("Waiting for read to complete\n");
        i2c_read_blocking(I2C_PORT, addr, &status,1, false);
    }

    i2c_read_blocking(I2C_PORT, addr, data, 6, false); // Read all data

    // Convert data for reading and store in variable
    // Based on Adafruit methodology for Arduino
    uint32_t humidity = data[1];
    humidity <<= 8;
    humidity |= data[2];
    humidity <<= 4;
    humidity |= data[3] >> 4;
    humReading = ((float)humidity * 100) / 1048576;

    uint32_t temp = data[3] & 0x0F;
    temp <<= 8;
    temp |= data[4];
    temp <<= 8;
    temp |= data[5];
    tempReading = ((float)temp * 200 / 1048576) - 50;
}

float AHT20::getTemp() {
    return tempReading;
}

float AHT20::getTemp_f() {
    return (tempReading * 1.8) + 32;
}

float AHT20::getHumidity() {
    return humReading;
}