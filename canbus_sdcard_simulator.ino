// Read SD Card, measure Vehicle signals, send them over CAN Messages
// SD Card should have a file called can.csv
// Convert any other dataformat into .csv 

#include <SPI.h>
#include <SD.h>
#include <CSV_Parser.h>
#include <CAN.h>
#include <Button.h>

// SD card 
File myFile;
bool sdFlag;

#define CAN_2515
// #define CAN_2518FD
// Set SPI CS Pin according to your hardware

#if defined(SEEED_WIO_TERMINAL) && defined(CAN_2518FD)
// For Wio Terminal w/ MCP2518FD RPi Hat：
// Channel 0 SPI_CS Pin: BCM 8
// Channel 1 SPI_CS Pin: BCM 7
// Interupt Pin: BCM25
const int SPI_CS_PIN  = BCM8;
const int CAN_INT_PIN = BCM25;
#else

// For Arduino MCP2515 Hat:
// the cs pin of the version after v1.1 is default to D9
// v0.9b and v1.0 is default D10
const int SPI_CS_PIN = 9;
const int CAN_INT_PIN = 2;
#endif

#ifdef CAN_2518FD
#include "mcp2518fd_can.h"
mcp2518fd CAN(SPI_CS_PIN); // Set CS pin
#endif

#ifdef CAN_2515
#include "mcp2515_can.h"
mcp2515_can CAN(SPI_CS_PIN); // Set CS pin
#endif

const int ledHIGH    = 1;
const int ledLOW     = 0;

Button button1(2);
Button button2(3); 

void setup() {
    SERIAL_PORT_MONITOR.begin(115200);
    attachInterrupt(digitalPinToInterrupt(CAN_INT_PIN), MCP2515_ISR, FALLING); // start interrupt
    while (CAN_OK != CAN.begin(CAN_250KBPS)) {             // init can bus : baudrate = 250k
        SERIAL_PORT_MONITOR.println("CAN init fail, retry...");
        delay(100);
    }
    SERIAL_PORT_MONITOR.println("CAN init ok!");
    
    if (!SD.begin(4)) {
        SERIAL_PORT_MONITOR.println("SD init fail!");
        while (1);
        sdFlag = false;

    }
    SERIAL_PORT_MONITOR.println("SD init OK.");
    sdFlag = true;

    if (SD.exists("can.csv"));{
        SERIAL_PORT_MONITOR.println("File exists");
        myFile = SD.open("can.csv", FILE_READ);
        }
    else {
        SERIAL_PORT_MONITOR.println("File not found!");
        SERIAL_PORT_MONITOR.println("");
    }
}

unsigned char stmp[8] = {ledHIGH, 1, 2, 3, ledLOW, 5, 6, 7};

int withSD_routine();
int noSD_routine();

void loop() {
  if (sdFlag == true){
    withSDroutine();
  }
  else {
    noSDroutine();
  }

    while (myFile.available()) {
      Serial.write(myFile.read());
    SERIAL_PORT_MONITOR.println("In loop");
    // send data:  id = 0x70, standard frame, data len = 8, stmp: data buf
    CAN.sendMsgBuf(0x70, 0, 8, stmp);
    delay(1000);                       // send data once per second
}


Split the program recursively starting by SD Card
SD CARD = TRUE
Send its contents over CAN bus one after the other 1 sec delay
1. Interupt by pressing any of the buttons   
2. Play signals and CAN messages alternating
3. Play all signals together and one SD Card message

SD CARD = FALSE
Send all the signals at once (or check the previous program)

int read_status(){

}
int withSD_routine(){
    READ SIGNAL STATUS
    SEND SIGNAL STATUS
    SEND SD CARD MSG
    return 0;
}

int noSD_routine(){
    READ SIGNAL STATUS
    SEND SIGNAL STATUS
    return 0;
}
