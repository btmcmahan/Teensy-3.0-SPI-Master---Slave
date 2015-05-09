
#include "t3spi.h"
#include "mk20dx128.h"
#include "core_pins.h"
#include "arduino.h"


T3SPI::T3SPI() {
	SIM_SCGC6 |= SIM_SCGC6_SPI0;	// enable clock to SPI.
	dataPointer=0;
	packetCT=0;
	ctar=0;
	delay(1000);
}

void T3SPI::begin_MASTER() {
	setMCR(MASTER);
	setCTAR(CTAR_0,8,SPI_MODE0,LSB_FIRST,SPI_CLOCK_DIV8);
	enablePins(SCK, MOSI, MISO, CS0, CS_ActiveLOW);
}

void T3SPI::begin_MASTER(uint8_t sck, uint8_t mosi, uint8_t miso, uint8_t cs, bool activeState){
	setMCR(MASTER);
	setCTAR(CTAR_0,8,SPI_MODE0,LSB_FIRST,SPI_CLOCK_DIV8);
	enablePins(sck, mosi, miso, cs, activeState);
}

void T3SPI::begin_SLAVE() {
	setMCR(SLAVE);
	setCTAR_SLAVE(8, SPI_MODE0);
	SPI0_RSER = 0x00020000;
	enablePins_SLAVE(SCK, MOSI, MISO, CS0);
}

void T3SPI::begin_SLAVE(uint8_t sck, uint8_t mosi, uint8_t miso, uint8_t cs) {
	setMCR(SLAVE);
	setCTAR_SLAVE(8, SPI_MODE0);
	SPI0_RSER = 0x00020000;
	enablePins_SLAVE(sck, mosi, miso, cs);
}

void T3SPI::setMCR(bool mode){
	stop();
	if (mode==1){
		SPI0_MCR=0x80000000;}
	else{
		SPI0_MCR=0x00000000;}
	start();
}

void T3SPI::setCTAR(bool CTARn, uint8_t size, uint8_t dataMode, uint8_t bo, uint8_t cdiv){
	if (CTARn == 0){
		SPI0_CTAR0=0;}
	if (CTARn == 1){
		SPI0_CTAR1=0;}
	setFrameSize(CTARn, (size - 1));
	setMode(CTARn, dataMode);
	setBitOrder(CTARn, bo);
	setClockDivider(CTARn, cdiv);
}

void T3SPI::setCTAR_SLAVE(uint8_t size, uint8_t dataMode){
	SPI0_CTAR0_SLAVE=0;
	setFrameSize(CTAR_SLAVE, (size - 1));
	setMode(CTAR_SLAVE, dataMode);
}

void T3SPI::enablePins(uint8_t sck, uint8_t mosi, uint8_t miso, uint8_t cs, bool activeState){
	if (sck == 0x0D){
		CORE_PIN13_CONFIG = PORT_PCR_DSE | PORT_PCR_MUX(2);}	//Set Pin13 Output & SCK
	if (sck == 0x0E){
		CORE_PIN13_CONFIG = PORT_PCR_MUX(1);
		CORE_PIN14_CONFIG = PORT_PCR_DSE | PORT_PCR_MUX(2);}	//Set Pin14 Output & SCK
	if (mosi == 0x0B){
		CORE_PIN11_CONFIG = PORT_PCR_DSE | PORT_PCR_MUX(2);}	//Set Pin11 Output & MOSI
	if (mosi ==  0x07){
		CORE_PIN7_CONFIG  = PORT_PCR_DSE | PORT_PCR_MUX(2);}	//Set Pin7  Output & MOSI
	if (miso == 0x0C){
		CORE_PIN12_CONFIG = PORT_PCR_MUX(2);}					//Set Pin12 Input & MISO
	if (miso ==  0x08){
		CORE_PIN8_CONFIG  = PORT_PCR_MUX(2);}					//Set Pin8  Input & MISO
	enableCS(cs, activeState);
}

void T3SPI::enableCS(uint8_t cs, bool activeState){
	if (cs   == 0x01){
		CORE_PIN10_CONFIG = PORT_PCR_DSE | PORT_PCR_MUX(2);		//Set Pin10 Output & CS0
		if (activeState == 1){
			setCS_ActiveLOW(CS0_ActiveLOW);}}
	if (cs   ==  0x02){
		CORE_PIN9_CONFIG  = PORT_PCR_DSE | PORT_PCR_MUX(2);		//Set Pin9  Output & CS1
		if (activeState == 1){
			setCS_ActiveLOW(CS1_ActiveLOW);}}
	if (cs   == 0x04){
		CORE_PIN20_CONFIG = PORT_PCR_DSE | PORT_PCR_MUX(2);		//Set Pin20 Output & CS2
		if (activeState == 1){
			setCS_ActiveLOW(CS2_ActiveLOW);}}
	if (cs   == 0x08){
		CORE_PIN21_CONFIG = PORT_PCR_DSE | PORT_PCR_MUX(2);		//Set Pin21 Output & CS3
		if (activeState == 1){
			setCS_ActiveLOW(CS3_ActiveLOW);}}
	if (cs   == 0x10){
		CORE_PIN15_CONFIG = PORT_PCR_DSE | PORT_PCR_MUX(2);		//Set Pin15 Output & CS4
		if (activeState == 1){
			setCS_ActiveLOW(CS4_ActiveLOW);}}
	if (cs   ==  0x81){
		CORE_PIN2_CONFIG  = PORT_PCR_DSE | PORT_PCR_MUX(2);		//Set Pin2  Output & (alt) CS0
		if (activeState == 1){
			setCS_ActiveLOW(CS0_ActiveLOW);}}
	if (cs   ==  0x82){
		CORE_PIN6_CONFIG  = PORT_PCR_DSE | PORT_PCR_MUX(2);		//Set Pin6  Output & (alt) CS1
		if (activeState == 1){
			setCS_ActiveLOW(CS1_ActiveLOW);}}
	if (cs   == 0x84){
		CORE_PIN23_CONFIG = PORT_PCR_DSE | PORT_PCR_MUX(2);		//Set Pin23 Output & (alt) CS2
		if (activeState == 1){
			setCS_ActiveLOW(CS2_ActiveLOW);}}
	if (cs   == 0x88){
		CORE_PIN22_CONFIG = PORT_PCR_DSE | PORT_PCR_MUX(2);		//Set Pin22 Output & (alt) CS3
		if (activeState == 1){
			setCS_ActiveLOW(CS3_ActiveLOW);}}
}

void T3SPI::enablePins_SLAVE(uint8_t sck, uint8_t mosi, uint8_t miso, uint8_t cs) {
    if (sck == SCK){
		CORE_PIN13_CONFIG = PORT_PCR_MUX(2);}
	if (sck == ALT_SCK){
		CORE_PIN14_CONFIG = PORT_PCR_MUX(2);}
	if (mosi == MOSI){
		CORE_PIN11_CONFIG = PORT_PCR_DSE | PORT_PCR_MUX(2);}
	if (mosi == ALT_MOSI){
		CORE_PIN7_CONFIG  = PORT_PCR_DSE | PORT_PCR_MUX(2);}
	if (miso == MISO){
		CORE_PIN12_CONFIG = PORT_PCR_MUX(2);}
	if (miso == ALT_MISO){
		CORE_PIN8_CONFIG  = PORT_PCR_MUX(2);}
	if (cs == CS0){
		CORE_PIN10_CONFIG = PORT_PCR_MUX(2);}
	if (cs == ALT_CS0){
		CORE_PIN2_CONFIG  = PORT_PCR_MUX(2);}
}

void T3SPI::setCS_ActiveLOW(uint32_t pin){
	stop();
	SPI0_MCR |= (pin);
	start();
}

void T3SPI::setFrameSize(uint8_t CTARn, uint8_t size) {
	stop();
	if (CTARn==0){
		SPI0_CTAR0 |= SPI_CTAR_FMSZ(size);}
	if (CTARn==1){
		SPI0_CTAR1 |= SPI_CTAR_FMSZ(size);}
	if (CTARn==2){
		SPI0_CTAR0_SLAVE |= SPI_CTAR_FMSZ(size);}
	start();
}
//(((size) & 31) << 27);

void T3SPI::setMode(uint8_t CTARn, uint8_t dataMode) {
	stop();
	if (CTARn==0){
		SPI0_CTAR0 = SPI0_CTAR0 & ~(SPI_CTAR_CPOL | SPI_CTAR_CPHA) | dataMode << 25;}
	if (CTARn==1){
		SPI0_CTAR1 = SPI0_CTAR1 & ~(SPI_CTAR_CPOL | SPI_CTAR_CPHA) | dataMode << 25;}
	if (CTARn==2){
		SPI0_CTAR0_SLAVE = SPI0_CTAR0_SLAVE & ~(SPI_CTAR_CPOL | SPI_CTAR_CPHA) | dataMode << 25;}
	start();
}

void T3SPI::setBitOrder(bool CTARn, uint8_t bo) {
	stop();
	if (CTARn==0){
		if (bo == LSBFIRST) {
			SPI0_CTAR0 |= SPI_CTAR_LSBFE;}
		if (bo == MSBFIRST) {
			SPI0_CTAR0 &= ~SPI_CTAR_LSBFE;}}
	if (CTARn==1){
		if (bo == LSBFIRST) {
			SPI0_CTAR1 |= SPI_CTAR_LSBFE;}
		if (bo == MSBFIRST) {
			SPI0_CTAR1 &= ~SPI_CTAR_LSBFE;}}
	start();
}

void T3SPI::setClockDivider(bool CTARn, uint8_t cdiv) {
	stop();
	if (CTARn==0){
		SPI0_CTAR0 |= SPI_CTAR_DBR | SPI_CTAR_CSSCK(cdiv) | SPI_CTAR_BR(cdiv);}
	if (CTARn==1){
		SPI0_CTAR1 |= SPI_CTAR_DBR | SPI_CTAR_CSSCK(cdiv) | SPI_CTAR_BR(cdiv);}
	start();
}

void T3SPI::start() {
	SPI0_MCR &= ~SPI_MCR_HALT & ~SPI_MCR_MDIS;
}

void T3SPI::stop() {
	SPI0_MCR |= SPI_MCR_HALT | SPI_MCR_MDIS;
}

void T3SPI::end() {
	SPI0_SR &= ~SPI_SR_TXRXS;
	stop();
}

void T3SPI::printStatistics(int length) {
	uint32_t CTAR=0;
	int bytesSent=0;
	float uSecsByte=0;
	float bytesSec=0;
	unsigned long timeElapsed=0;
	float Mbps=0;

	if (((SPI0_CTAR0 & 0x78000000)>>27)==7){
		bytesSent = (packetCT * length);}
	if (((SPI0_CTAR0 & 0x78000000)>>27)==15){
		bytesSent = (packetCT * length*2);}
	timeElapsed = timeStamp2-timeStamp1;
	uSecsByte= ((float)timeElapsed / (float)bytesSent);
	Mbps= ((float)bytesSent) / (float)timeElapsed;
	Serial.println();
	Serial.println();
	Serial.println("                 33222222222211111111110000000000");
	Serial.println("                 10987654321098765432109876543210");
	Serial.println("                 --------------------------------");
	Serial.print("SPIO_MCR:        ");
	for (unsigned int mask = 0x80000000; mask; mask >>= 1) {
		Serial.print(mask&SPI0_MCR?'1':'0');}
	Serial.println();
	
	if(0x80000000 & SPI0_MCR){
		if (ctar==0){
			Serial.print("SPIO_CTAR0:      ");		
			CTAR = SPI0_CTAR0;}
		if (ctar==1){
			Serial.print("SPIO_CTAR1:      ");
			CTAR = SPI0_CTAR1;}}
		else {
			Serial.print("SPIO_CTAR_SLAVE: ");
			CTAR = SPI0_CTAR0_SLAVE;}

	for (unsigned int mask = 0x80000000; mask; mask >>= 1) {
		Serial.print(mask&CTAR?'1':'0');}
			
	Serial.println();
	Serial.print("SPI0_SR:         ");
	for (unsigned int mask = 0x80000000; mask; mask >>= 1) {
		Serial.print(mask&SPI0_SR?'1':'0');}
	Serial.println();
	Serial.print("SPI0_RSER:       ");
	for (unsigned int mask = 0x80000000; mask; mask >>= 1) {
		Serial.print(mask&SPI0_RSER?'1':'0');}
	Serial.println();
	Serial.println();
	Serial.print("Frame Size:      ");
	Serial.println(((CTAR & 0x78000000)>>27)+1);
	Serial.print("Data Length:     ");
	Serial.println(length);
	Serial.print("Packets:         ");
	Serial.println(packetCT);
	Serial.print("Bytes Sent:      ");
	Serial.println(bytesSent);
	Serial.print("Time Elapsed:    ");
	Serial.println(timeElapsed);
	Serial.print("uSecs/Byte:      ");
	Serial.println(uSecsByte);
	Serial.print("Mbps:            ");
	Serial.println(Mbps);
	Serial.println();
	Serial.flush();
}

//TRANSMIT PACKET OF 8 BIT DATA
void T3SPI::tx8(volatile uint8_t *dataOUT,   int length, bool CTARn, uint8_t PCS){ 
	ctar=CTARn;
	for (int i=0; i < length; i++){
		SPI_WRITE_8(dataOUT[i], CTARn, PCS);
		SPI_WAIT();}
	packetCT++;
}

//TRANSMIT PACKET OF 16 BIT DATA
void T3SPI::tx16(volatile uint16_t *dataOUT, int length, bool CTARn, uint8_t PCS){ 
	//ctar=CTARn;
	for (int i=0; i < length; i++){
		SPI_WRITE_16(dataOUT[i], CTARn, PCS);
		SPI_WAIT();}
	packetCT++;
}

//TRANSMIT & RECEIVE PACKET OF 8 BIT DATA
void T3SPI::txrx8(volatile uint8_t *dataOUT, volatile uint8_t *dataIN, int length, bool CTARn, uint8_t PCS){ 
	ctar=CTARn;
	for (int i=0; i < length; i++){
		SPI0_MCR |= SPI_MCR_CLR_RXF;
		SPI_WRITE_8(dataOUT[i], CTARn, PCS);
		SPI_WAIT();
		delayMicroseconds(1);
		dataIN[i]=SPI0_POPR;
		}
	packetCT++;
}

//TRANSMIT & RECEIVE PACKET OF 16 BIT DATA
void T3SPI::txrx16(volatile uint16_t *dataOUT, volatile uint16_t *dataIN, int length, bool CTARn, uint8_t PCS){ 
	ctar=CTARn;
	for (int i=0; i < length; i++){
		SPI0_MCR |= SPI_MCR_CLR_RXF;
		SPI_WRITE_16(dataOUT[i], CTARn, PCS);
		SPI_WAIT();
		delayMicroseconds(1);
		dataIN[i]=SPI0_POPR;
		}
	packetCT++;
}

void T3SPI::rx8(volatile uint8_t *dataIN, int length){
	dataIN[dataPointer] = SPI0_POPR;
	dataPointer++;
	if (dataPointer == length){
		dataPointer=0;
		packetCT++;}
	SPI0_SR |= SPI_SR_RFDF;
}

void T3SPI::rx16(volatile uint16_t *dataIN, int length){
	dataIN[dataPointer] = SPI0_POPR;
	dataPointer++;
	if (dataPointer == length){
		dataPointer=0;
		packetCT++;}
	SPI0_SR |= SPI_SR_RFDF;
}

void T3SPI::rxtx8(volatile uint8_t *dataIN, volatile uint8_t *dataOUT, int length){
	dataIN[dataPointer] = SPI0_POPR;
	dataPointer++;
	if (dataPointer == length){
		dataPointer=0;
		packetCT++;}
	SPI0_PUSHR_SLAVE = dataOUT[dataPointer];  
	SPI0_SR |= SPI_SR_RFDF;
}

void T3SPI::rxtx16(volatile uint16_t *dataIN, volatile uint16_t *dataOUT, int length){
	dataIN[dataPointer] = SPI0_POPR;
	dataPointer++;
	if (dataPointer == length){
		dataPointer=0;
		packetCT++;}
  SPI0_PUSHR_SLAVE = dataOUT[dataPointer];  
  SPI0_SR |= SPI_SR_RFDF;
}