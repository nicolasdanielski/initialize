#include "init.h"

String NMEA1;  // we will use this variable to hold our first NMEA sentence
String NMEA2;  // we will use this variable to hold our second NMEA sentence
char c;       // used to read the characters spewing from the GPS module

void readGPS(); // will read the NMEA sentences coming from the sensor and use library parser for easy access to latitude, longitude and altitute

void cleanGPS(); // clean the serial buffer

void parse_initial_data(); // parse all flags and send to control