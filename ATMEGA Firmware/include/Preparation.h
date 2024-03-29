//Importando Bibliotecas
#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <A4988.h>
#include <hx711.h>
#include <EEPROM.h>

//Motor Configuration
#define MOTOR_STEPS 200                             //Motor steps for full revolution
#define MOTOR_ACCEL 15000                           //Motor acceleration mm/s²
#define MOTOR_DECEL 15000                           //Motor deceleration mm/s²
#define MICROSTEPS 8
                               //Driver microsteps configuration
#define DIR 13                                      //Direction pin
#define STEP 12                                     //Step pin
#define ENABLE 11                                   //Enable pin
#define RPM 60                                      //Initial RPM setting
A4988 stepper(MOTOR_STEPS, DIR, STEP, ENABLE);      //Initializing Stepper

//SD Card Configuration
const int chipSelect = 53;                          //SD Card SSDSS pin
File dataFile;                                      //Set up variables using the SD utility library functions

//Load Cell Configuration
const int LOADCELL_DOUT_PIN = 7;                   //Data pin
const int LOADCELL_SCK_PIN = 6;                    //Clock pin
HX711 loadCell;                                     //Initializing loadcell

//End Stop Switch
const int Y_END_STOP = 14;
const int Z_END_STOP = 15;

//Global Positioning System
float X = 0;
const float Neutral_Position = 54;
unsigned long time_millis = 0;
boolean home = false;

