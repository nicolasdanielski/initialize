#include "com.h"

void clearGPS() {  // since between GPS reads, we still have data streaming in, we need to clear the old data by reading a few sentences, and discarding these
    while(!mtk3339.newNMEAreceived()) {
        c=mtk3339.read();
    }
    mtk3339.parse(mtk3339.lastNMEA());
}

void readGPS(){  // this function will read and remember two NMEA sentences from GPS
    clearGPS();    // serial port probably has old or corrupt data, so begin by clearing it all out
    while(!mtk3339.newNMEAreceived()) { // keep reading characters in this loop until a good NMEA sentence is received
        c=mtk3339.read(); // read a character from the GPS
    }
    mtk3339.parse(mtk3339.lastNMEA());  // once you get a good NMEA, parse it
    NMEA1=mtk3339.lastNMEA();      // once parsed, save NMEA sentence into NMEA1
    while(!mtk3339.newNMEAreceived()) {  // go out and get the second NMEA sentence, should be different type than the first one read above.
        c=mtk3339.read();
    }
    mtk3339.parse(mtk3339.lastNMEA());
    NMEA2=mtk3339.lastNMEA();
    /*Serial.println(NMEA1);
    Serial.println(NMEA2);
    Serial.print("latitude:       "); Serial.print(mtk3339.latitude, 5); Serial.println(mtk3339.lat);
    Serial.print("longitude:      "); Serial.print(mtk3339.longitude, 5); Serial.println(mtk3339.lon);
    Serial.print("altitude:       "); Serial.println(mtk3339.altitude);
    Serial.print("Speed (knots):  "); Serial.println(mtk3339.speed);
    Serial.print("satellites:     "); Serial.println((int)mtk3339.satellites);*/ //needs a decent parser :/
}

void parse_initial_data(){
    uint8_t data[RH_RF95_MAX_MESSAGE_LEN]; // data "stream"

    if (f_sensors_ok) { // checks for sensors initialization
        sprintf(reinterpret_cast<char*>(data), "All sensors have been initialized"); // parsing (may bug)
    } else {
        sprintf(reinterpret_cast<char*>(data), "A error ocurred while initializing sensors");
    }
    rf95.send(data, sizeof(data)); // send to control
                                   // all other follow the same structure as here
    if (f_all_sensors) {
        sprintf(reinterpret_cast<char*>(data), "All sensors have been found");
    } else {    
        sprintf(reinterpret_cast<char*>(data), "Not all sensors have been found");
    }
    rf95.send(data, sizeof(data));

    sprintf(reinterpret_cast<char*>(data), "Sensors found at addresses: %x %x %x %x", sensor_count[0], sensor_count[1], sensor_count[2], sensor_count[3]);
    rf95.send(data, sizeof(data));

    sprintf(reinterpret_cast<char*>(data), "Errors found at addresses: %x %x %x %x", sensor_error[0], sensor_error[1], sensor_error[2], sensor_error[3]);
    rf95.send(data, sizeof(data));

    if (f_sd_ok) {
        sprintf(reinterpret_cast<char*>(data), "SD card is okay");
    } else {
        sprintf(reinterpret_cast<char*>(data), "SD card is not okay");
    }
    rf95.send(data, sizeof(data));
    if (f_lora_ok) {
        sprintf(reinterpret_cast<char*>(data), "LoRa com is okay (evidently)");
        rf95.send(data, sizeof(data));
    } else {
        ;  // ? we already have the buzzer for this case
    }
}