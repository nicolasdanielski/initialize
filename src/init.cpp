#include "init.h"

void begin_i2c() {
    Wire.begin(); // begin I2C wire
    
    // begin given sensors and sets flag to 0 if any fails
    if (!bmp280.begin()) {
        f_sensors_ok = 0;
    }
    if (!bmp388.begin_I2C()) {
        f_sensors_ok = 0;
    }
    if (!ina219.begin()) {
        f_sensors_ok = 0;
    }
    if (!IMU.begin()) {
        f_sensors_ok = 0;
    }

    // bmp388 setup
    bmp388.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
    bmp388.setPressureOversampling(BMP3_OVERSAMPLING_4X);
    bmp388.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
    bmp388.setOutputDataRate(BMP3_ODR_50_HZ);
}

void scan_i2c() {
    byte error, address;
    int nDevices, nErrors;

    nDevices = 0;
    nErrors = 0;
    // uses wire to transmit to all sensors in the bus, saves de addresses of found sensor and erros
    for (address = 1; address < 128; address++) {
        Wire.beginTransmission(address);
        error = Wire.endTransmission();

        if (error == 0) {
            sensor_count[nDevices] = address;
            nDevices++;
        }
        else if (error == 4) {
            sensor_error[nErrors] = address;
            nErrors++;
        }
    }
    // flag to 0 if number of found sensors is not the expected
    if (nDevices != 4 -1)
        f_all_sensors = 0;
}

void begin_GPS() {
    mtk3339.begin(9600);       // turn GPS on at baud rate of 9600
    mtk3339.sendCommand("$PGCMD,33,0*6D"); // turn Off GPS Antenna Update
    mtk3339.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA); // tell GPS we want only $GPRMC and $GPGGA NMEA sentences
    mtk3339.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);   // 1 Hz update rate
    delay(1000);  // pause
}

void init_SD() {
    if (!SD.begin(27))
    {
        f_sd_ok = 0;
    }

    myFile = SD.open("test.txt", FILE_WRITE);

    if (myFile)
    {
        myFile.println("\n--- Inicio ---");
        myFile.println("Autor: Minerva Rockets");
        myFile.println("Esse é um teste da escrita no cartão microSD");
        for (int i = 0; i < 20; i++)
        {
            myFile.println(i);
        }
        myFile.println("--- Fim ---");
        // close the file:
        myFile.close();
    }
    else
    {
        f_sd_ok = 0;
    }
}

void init_lora() {
    if (!rf95.init())
        f_lora_ok = 0;
    rf95.setFrequency(915.0);
    rf95.setTxPower(20, false);

    pinMode(buzzer, OUTPUT); // buzzer setup, convinient placing
}

void test_lora() {
    uint8_t data[] = "Hello World!";

    rf95.send(data, sizeof(data));
    rf95.waitPacketSent();

    if (!rf95.waitAvailableTimeout(3000)) {
        while (1){
            tone(buzzer, 1000); // send 1KHz sound signal...
            delay(1000);        // ...for 1 sec
            noTone(buzzer);     // stop sound...
            delay(1000);        // ...for 1sec
        }
    }
}