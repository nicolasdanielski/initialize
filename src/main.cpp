#include "init.h"
#include "com.h"

void setup() {
    begin_i2c(); // begin i2c
    scan_i2c(); // scan for i2c addresses

    begin_GPS(); // begin gps

    init_SD(); // init SD card and tests

    init_lora(); // init LoRa com
    test_lora(); // send and wait for message receiving

    parse_initial_data(); // parse all flags regarding system health
}