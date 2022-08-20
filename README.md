# AHT20-For-Pico
C++ implementation of AHT20 for RP2040 Picos

## Basic usage
1. Include header file
2. Create AHT20 object
3. Initialise sensor with .init(sdaPort, sclPort)
   1. provide phyiscal pin numbers for I2C SDA and SCL ports
4. Trigger a measurement with .triggerMeasurement()
5. Read latest temperature (celsius) as float with .getTemp()
   1. temperature in farenheit can be read with .getTemp_f()
6. read latest humidity as float with .getHumidity()

## Example
 
    include <stdio.h>
    include pico/stdlib.h
    include libraries/AHT20/AHT20.h
    AHT20 aht20;
    int temp;
    int hum;
    
    int main() {
        stdio_init_all();
        aht20.init(0,1);
    
        while (true) {
            sleep_ms(10000);
            aht20.triggerMeasurement();
    
            temp = (int)aht20.getTemp();
            hum = (int)aht20.getHumidity();
    
            printf("Temp: %iC \n", temp);
            printf("Humidity: %i%%\n", hum);
        }
    }

### Output
    Temp: 23C
    Temp: 74f
    Humidity: 46%