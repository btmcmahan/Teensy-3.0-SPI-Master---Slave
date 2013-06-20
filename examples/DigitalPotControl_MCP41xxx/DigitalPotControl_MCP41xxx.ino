/* This example shows using the SPI_WRITE_16 or SPI_WRITE_8 
to send data that isn't in array form.
*/


#include <t3spi.h>

//Initialize T3SPI class as SPI_MASTER
T3SPI SPI_MASTER;

//Initialize the arrays for outgoing data
volatile uint16_t data;

void setup(){
  
  //Begin SPI in MASTER (SCK pin, MOSI pin, MISO pin, CS pin, Active State)
  SPI_MASTER.begin_MASTER(SCK, MOSI, MISO, CS0, CS_ActiveLOW);
  
  //Set the CTAR (CTARn, Frame Size, SPI Mode, Shift Mode, Clock Divider) 
  SPI_MASTER.setCTAR(CTAR0,16,SPI_MODE0,LSB_FIRST,SPI_CLOCK_DIV8);
}

void loop(){

  for (int i=150; i<256; i++) {
    
    //this combines the control byte (0x11) and the data (0xff)
    // into one 16 bit byte (0x11ff)
    data =(0x1100 | i);
    
    //SPI_WRITE_16( uint16_t data, CTARnum, CS pin)
    SPI_WRITE_16(data, 0, 1);
    
    delay(15);
  }


  for (int i=255; i >=150; i--) {
    
    //this combines the control byte (0x11) and the data (0xff)
    // into one 16 bit byte (0x11ff)
    data =(0x1100 | i);
    
    //SPI_WRITE_16( uint16_t data, CTARnum, CS pin)
    SPI_WRITE_16(data, 0, 1);
    
    delay(15);
  }  
  
}
