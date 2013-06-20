
#include <t3spi.h>

//Initialize T3SPI class as SPI_SLAVE
T3SPI SPI_SLAVE;

//The number of integers per data packet
//MUST be the same as defined on the MASTER device
#define dataLength  256

//Initialize the arrays for incoming data
//volatile uint8_t data[dataLength] = {};
volatile uint16_t data[dataLength] = {};


void setup(){
  
  Serial.begin(115200);
  
  //Begin SPI in SLAVE (SCK pin, MOSI pin, MISO pin, CS pin)
  SPI_SLAVE.begin_SLAVE(SCK, MOSI, MISO, CS0);
  
  //Set the CTAR0_SLAVE0 (Frame Size, SPI Mode)
  //SPI_SLAVE.setCTAR_SLAVE(8, SPI_MODE0);
  SPI_SLAVE.setCTAR_SLAVE(16, SPI_MODE0);
  
  //Enable the SPI0 Interrupt
  NVIC_ENABLE_IRQ(IRQ_SPI0);
  
}

void loop(){

  //Capture the time before receiving data
  if (SPI_SLAVE.dataPointer==0 && SPI_SLAVE.packetCT==0){
    SPI_SLAVE.timeStamp1=micros();}  
 
  //Capture the time when transfer is done
  if (SPI_SLAVE.packetCT==1){
    SPI_SLAVE.timeStamp2=micros();

    //Print data received & data sent
    for (int i=0; i<dataLength; i++){
      Serial.print("data[");
      Serial.print(i);
      Serial.print("]: ");
      Serial.println(data[i]);
      Serial.flush();}
  
      //Print statistics for the previous transfer
      SPI_SLAVE.printStatistics(dataLength); 
   
    //Reset the packet count   
    SPI_SLAVE.packetCT=0;}
}

//Interrupt Service Routine to handle incoming data
void spi0_isr(void){
  
  //Function to handle data
  //SPI_SLAVE.rx8 (data, dataLength);
  SPI_SLAVE.rx16(data, dataLength);
}
