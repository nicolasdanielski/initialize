#define SEALEVEL_PRESSURE_HPA (1011)
#define USE_SW_SERIAL

#include <Arduino.h>

#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SoftwareSerial.h>
#include <RH_RF95.h>

#include "Adafruit_Sensor.h"
#include "Adafruit_BMP280.h"
#include "Adafruit_BMP3XX.h"
#include "Adafruit_INA219.h"
#include "Adafruit_GPS.h"

#include "MPU9250.h"

// base code for gps init and parsing: https://github.com/Oliverpxrxz17/MTK3339-GPS-code/blob/main/GPSGGARMC.ino
SoftwareSerial gpsSerial(0, 1); // setup serial com with the mtk3339
Adafruit_GPS mtk3339(&gpsSerial); // create GPS object

Adafruit_BMP280 bmp280; // bmp280 sensor
Adafruit_BMP3XX bmp388; // bmp388 sensor
Adafruit_INA219 ina219; // ina219 sensor
MPU9250 IMU(Wire,0x68); // mpu9250 sensor

File myFile; // for opening files in the SD card

RH_RF95 rf95(24, 26); // rf95 object with CS in 24 and G0 in 26
int buzzer = 32; // buzzer pintout

int f_sensors_ok = 1; // flag, goes to 0 if any sensor fails to begin
int f_all_sensors = 1; // flag, goes to 0 if number of initalized sensor is not the expected
int f_sd_ok = 1; // flag, goes to 0 if SD card fails to initialize or if test fails
int f_lora_ok = 1; // flag, goes to 0 if fails to initialize lora 
byte sensor_count[] = {0, 0, 0, 0}; // vector with found sensors addresses
byte sensor_error[] = {0, 0, 0, 0}; // vector of addresses with errors

void begin_i2c(); // i2c initializing function
void scan_i2c(); // scan for i2c addresses
void begin_GPS(); // GPS begin and fix
void init_SD(); // init SD serial and checks if it went ok
void init_lora(); // init lora com
void test_lora(); // test if lora com is stablished with the base