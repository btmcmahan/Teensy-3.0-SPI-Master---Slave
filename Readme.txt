This library was written specifically for the TEENSy 3.0 SPI.  It will not work on Arduino or any other system.

This library supports using the alternate SPI pins, the gray pins on the pinout diagram.

***
It should be noted that I experienced some problems when using SLAVE mode, with 8 bit frames,
with a large array length, at the fastest clock rate.  I was able to go to the next clock
divider (SPI_CLOCK_DIV4	= 12.0	MHz) and this solved my problem.  I can also use a smaller
array length with no issues.  I think that at very high speed, with short frame size, the
master SPI can over-run my slave ISR scan time. 

***
The SLAVE "return data" is loaded every time the ISR is called.  So on the first byte received,
there will be no data loaded into the "return data" register.  So expect no return data (zero's)
on the very first frame transferred.

***
Many slave devices require that the CS pin stay active until several bytes have been transfered.
For these, it may be useful to specify your own CS pin and do a digitalWrite(CSpin, state) outside
of the tx function.

Example:
	digitalWrite(CS18, LOW);
	t3SPI.tx8(data1, CTARn, CS);
	t3SPI.tx8(data2, CTARn, CS);
	digitalWrite(CS18, HIGH);




******* F U N C T I O N     D E F I N I T I O N S *******


********************************************
******* FUNCTIONS  FOR  MASTER  MODE *******


begin_MASTER();

*******	This funtion initializes the SPI(0) to its most basic configuration.

	SPI:		Master Mode
	CTAR:		CTAR0
	Clock Divider: 	8 = 5.3 Mhz
	Frame Size:	8 bits
	SPI Mode:	0
	Bit Order:	LSB First
	CS State:	Active Low

	Clock:		Pin 13 (output)
	MOSI:		Pin 11 (output)
	MISO:		Pin 12 (intput)
	CS0:		Pin 10 (output)


	
begin_MASTER(sck, mosi, miso, cs, activeState);

*******	This funtion initializes the SPI(0) with user-defined pins.
	You can initialize Alternate Pins here.
	
	sck	OPTIONS:	SCK	 = Pin 13
				ALT_SCK  = Pin 14

	mosi	OPTIONS:	MOSI	 = Pin 11
				ALT_MOSI = Pin  7

	miso	OPTIONS:	MISO	 = Pin 12
				ALT_MISO = Pin  8

	cs	OPTIONS:	CS0	 = Pin 10
				CS1	 = Pin  9
				CS2	 = Pin 20
				CS3 	 = Pin 21
				CS4	 = Pin 15
				ALT_CS0  = Alternate CS0 on Pin  2
				ALT_CS1  = Alternate CS1 on Pin  6
				ALT_CS2  = Alternate CS2 on Pin 23
				ALT_CS3  = Alternate CS3 on Pin 22

	activeState OPTIONS:	ActiveLOW  = CS pin normally HIGH, goes LOW durring transfer
				ActiveHIGH = CS pin normally LOW, goes HIGH durring transfer



setCTAR(CTARn, size, dataMode, bo, cdiv);

*******	This funtion configures the CTARn
	The CTAR's specify how data will be sent out.
	There are two CTAR registers, CTAT0 and CTAR1.
	This is useful when you are using one Master to communicate to multiple slave devices.

	CTARn	OPTIONS:	CTAR0	= configures CTAR0
				CTAR1	= configures CTAR1

	size	OPTIONS:	8	= sets a frame size of  8 bits (per transfer)
				16	= sets a frame size of 16 bits (per transfer)

	dataMode OPTIONS:	0 - 3	= industry standard SPI clock modes, see wikipedia SPI Mode

	bo	OPTIONS:	LSB_FIRST = Least significant bit transfered first
				MSB_FIRST = Most significant bit transfered first.

	cdiv	OPTIONS:	#define SPI_CLOCK_DIV2 0b0000	//24.0	MHz
				SPI_CLOCK_DIV4	= 12.0	MHz
				SPI_CLOCK_DIV6	= 08.0	MHz
				SPI_CLOCK_DIV8	= 05.3	MHz
				SPI_CLOCK_DIV16	= 03.0	MHz
				SPI_CLOCK_DIV32	= 01.5	MHz
				SPI_CLOCK_DIV64	= 750	KHz
				SPI_CLOCK_DIV128= 375	KHz
				There are more modes available, but not includeded in this library.



enableCS(cs, activeState);
*******	This funtion enables CS pins.
	This is useful if you want to use more CS pins to control multiple SPI devices.
	You can initialize Alternate Pins here.
	
	cs	OPTIONS:	CS0	 = Pin 10
				CS1	 = Pin  9
				CS2	 = Pin 20
				CS3 	 = Pin 21
				CS4	 = Pin 15
				ALT_CS0  = Alternate CS0 on Pin  2
				ALT_CS1  = Alternate CS1 on Pin  6
				ALT_CS2  = Alternate CS2 on Pin 23
				ALT_CS3  = Alternate CS3 on Pin 22

	activeState OPTIONS:	ActiveLOW  = CS pin normally HIGH, goes LOW durring transfer
				ActiveHIGH = CS pin normally LOW, goes HIGH durring transfer



tx8(data, length, CTARn, PCS);

*******	This funtion sends a packet of 8 bit data

	dataOUT	OPTIONS:	This is where you put the name of your (8 bit) outgoing data array

	length	OPTIONS:	Put the length of the outgoing data array here

	CTARn	OPTIONS:	CTAR0	:Use the attributes that were set in CTAR0
				CTAR1	:Use the attributes that were set in CTAR1

	PCS	OPTIONS:	Any CS pin what has been enabled. (example: CS0, ALT_CS2)



tx16(data, length, CTARn, PCS);

*******	This funtion sends a packet of 16 bit data

	dataOUT	OPTIONS:	This is where you put the name of your (16 bit) outgoing data array

	length	OPTIONS:	Put the length of the outgoing data array here

	CTARn	OPTIONS:	CTAR0	:Use the attributes that were set in CTAR0
				CTAR1	:Use the attributes that were set in CTAR1

	PCS	OPTIONS:	Any CS pin what has been enabled. (example: CS0, ALT_CS2)



txrx8(dataOUT, dataIN, length, CTARn, PCS);

*******	This funtion sends a packet of 8 bit data & receives a packet of 8 bit data from the slave device

	dataOUT	OPTIONS:	This is where you put the name of your (8 bit) outgoing data array

	dataIN	OPTIONS:	This is where you put the name of your (8 bit) incoming data array

	length	OPTIONS:	Put the length of the outgoing data array here

	CTARn	OPTIONS:	CTAR0	:Use the attributes that were set in CTAR0
				CTAR1	:Use the attributes that were set in CTAR1

	PCS	OPTIONS:	Any CS pin what has been enabled. (example: CS0, ALT_CS2)



txrx16(dataOUT, dataIN, length, CTARn, PCS);

*******	This funtion sends a packet of 16 bit data & receives a packet of 16 bit data from the slave device

	dataOUT	OPTIONS:	This is where you put the name of your (16 bit) outgoing data array

	dataIN	OPTIONS:	This is where you put the name of your (16 bit) incoming data array

	length	OPTIONS:	Put the length of the outgoing data array here

	CTARn	OPTIONS:	CTAR0	:Use the attributes that were set in CTAR0
				CTAR1	:Use the attributes that were set in CTAR1

	PCS	OPTIONS:	Any CS pin what has been enabled. (example: CS0, ALT_CS2)




********************************************
******* FUNCTIONS  FOR  SLAVE  MODE ********


begin_SLAVE();

*******	This funtion initializes the SPI(0) to its most basic configuration.

	SPI:		SLAVE Mode
	CTAR:		CTAR0_SLAVE
	Frame Size:	8 bits
	SPI Mode:	0
	
	Clock:		Pin 13 (input)
	MOSI:		Pin 11 (output)
	MISO:		Pin 12 (input)
	CS0:		Pin 10 (input)



begin_SLAVE(sck, mosi, miso, cs);

*******	This funtion initializes the SPI(0) with user-defined pins.
	You can initialize Alternate Pins here.
	CS is always CS0 (or ALT_CS0) & ACTIVE_LOW in slave mode.
	
	sck	OPTIONS:	SCK	 = Pin 13
				ALT_SCK  = Pin 14

	mosi	OPTIONS:	MOSI	 = Pin 11
				ALT_MOSI = Pin  7

	miso	OPTIONS:	MISO	 = Pin 12
				ALT_MISO = Pin  8

	cs	OPTIONS:	CS0	 = Pin 10
				ALT_CS0  = Alternate CS0 on Pin  2



setCTAR_SLAVE(size, dataMode);

*******	This funtion configures the CTAR0_SLAVE.
	The CTAR's specify how data will be received.
	SPI0_CTAR0_SLAVE is the CTAR used in SLAVE Mode.
	Data is always shifted out LSB_FIRST in SLAVE Mode.
	The Frame Size MUST be the same as defined on the MASTER device.
	The dataMode should be the same as defined on the MASTER device.


	size	OPTIONS:	8	= sets a frame size of  8 bits (per transfer)
				16	= sets a frame size of 16 bits (per transfer)

	dataMode OPTIONS:	0 - 3	= industry standard SPI clock modes, see wikipedia SPI Mode



******* NOTE *******
The SPI SLAVE uses an Interrupt Service Routine to notify the device that a frame of data has been received.
A rx function must be used in ISR to tell the device how to handle it received data. 

It looks like this (example):

void spi0_isr(void){
	rx8(outgoingData, dataLength);}

The following 4 functions can be put in the ISR to handle incoming data.  Do not use more than rx function in the ISR.



rx8(dataIN, length);

*******	This funtion receives a packet of 8 bit data

	dataIN	OPTIONS:	This is where you put the name of your (8 bit) incoming data array

	length	OPTIONS:	Put the length of the incoming data array here



rx16(dataIN, length);

*******	This funtion receives a packet of 16 bit data

	dataIN	OPTIONS:	This is where you put the name of your (16 bit) incoming data array

	length	OPTIONS:	Put the length of the incoming data array here



rxtx8(dataIN, dataOUT, length);

*******	This funtion receives a packet of 8 bit data & returns a packet of 8 bit data to the master device

	dataIN	OPTIONS:	This is where you put the name of your (8 bit) incoming data array

	dataOUT	OPTIONS:	This is where you put the name of your (8 bit) outgoing data array

	length	OPTIONS:	Put the length of the incoming data array here



rxtx16(dataIN, dataOUT, length);

*******	This funtion receives a packet of 16 bit data & returns a packet of 16 bit data to the master device

	dataIN	OPTIONS:	This is where you put the name of your (16 bit) incoming data array

	dataOUT	OPTIONS:	This is where you put the name of your (16 bit) outgoing data array

	length	OPTIONS:	Put the length of the incoming data array here



********************************************
************* GLOBAL FUNCTIONS *************


start();

*******	This enables SPI functions.



stop();

*******	This halts SPI functions.



end();

*******	This disables SPI functions.
	


printStatistics(length);

*******	This function prints certain SPI statistics to the Serial monitor.
	You must set the following variables to get acurate data:

	T3SPI.timeStamp1=micros();
	T3SPI.timeStamp2=micros();

	length	OPTIONS:	Put the length of the outgoing or incoming data array here



********************************************
************ PRIVATE FUNCTIONS *************

*******	These functions cannot be used in the sketch but are used within the class.


setMCR(mode);

*******	This funtion initializes the SPI(0) to basic Master or Slave Mode.

	mode	OPTIONS:	MASTER = Master SPI Mode
				SLAVE  = Slave  SPI Mode



setFrameSize(CTARn, size);

*******	This funtion sets the frame size.
	
	CTARn	OPTIONS:	CTAR0		= configures CTAR0
				CTAR1		= configures CTAR1
				CTAR_SLAVE	= configures CTAR0_SLAVE

	size	OPTIONS:	8	= sets a frame size of  8 bits (per transfer)
				16	= sets a frame size of 16 bits (per transfer)


setMode(CTARn, dataMode);

*******	This funtion sets the spi data mode.

	CTARn	OPTIONS:	CTAR0		= configures CTAR0
				CTAR1		= configures CTAR1
	
	dataMode OPTIONS:	0 - 3	= industry standard SPI clock modes, see wikipedia SPI Mode



setBitOrder(CTARn, bo);

*******	This funtion sets the outgoing bit order.

	CTARn	OPTIONS:	CTAR0		= configures CTAR0
				CTAR1		= configures CTAR1

	bo	OPTIONS:	LSB_FIRST = Least significant bit transfered first
				MSB_FIRST = Most significant bit transfered first.

	
setClockDivider(CTARn, cdiv);

*******	This funtion sets the clock divider in MASTER mode.


	CTARn	OPTIONS:	CTAR0		= configures CTAR0
				CTAR1		= configures CTAR1

	cdiv	OPTIONS:	#define SPI_CLOCK_DIV2 0b0000	//24.0	MHz
				SPI_CLOCK_DIV4	= 12.0	MHz
				SPI_CLOCK_DIV6	= 08.0	MHz
				SPI_CLOCK_DIV8	= 05.3	MHz
				SPI_CLOCK_DIV16	= 03.0	MHz
				SPI_CLOCK_DIV32	= 01.5	MHz
				SPI_CLOCK_DIV64	= 750	KHz
				SPI_CLOCK_DIV128= 375	KHz
				There are more modes available, but not includeded in this library.



enablePins(sck, mosi, miso, cs, activeState);	

*******	This funtion enables the SPI pins
	You can initialize Alternate Pins here.

	sck	OPTIONS:	SCK	 = Pin 13
				ALT_SCK  = Pin 14

	mosi	OPTIONS:	MOSI	 = Pin 11
				ALT_MOSI = Pin  7

	miso	OPTIONS:	MISO	 = Pin 12
				ALT_MISO = Pin  8

	cs	OPTIONS:	CS0	 = Pin 10
				CS1	 = Pin  9
				CS2	 = Pin 20
				CS3 	 = Pin 21
				CS4	 = Pin 15
				ALT_CS0  = Alternate CS0 on Pin  2
				ALT_CS1  = Alternate CS1 on Pin  6
				ALT_CS2  = Alternate CS2 on Pin 23
				ALT_CS3  = Alternate CS3 on Pin 22

	activeState OPTIONS:	ActiveLOW  = CS pin normally HIGH, goes LOW durring transfer
				ActiveLOW  = CS pin normally LOW, goes HIGH durring transfer



enablePins_SLAVE(sck, mosi, miso, cs);

*******	This funtion enables the SPI pins for SLAVE mode.
	You can initialize Alternate Pins here.

	sck	OPTIONS:	SCK	 = Pin 13
				ALT_SCK  = Pin 14

	mosi	OPTIONS:	MOSI	 = Pin 11
				ALT_MOSI = Pin  7

	miso	OPTIONS:	MISO	 = Pin 12
				ALT_MISO = Pin  8

	cs	OPTIONS:	CS0	 = Pin 10
				ALT_CS0  = Alternate CS0 on Pin  2


setCS_ActiveLOW(pin);

*******	This funtion sets CS pins as ACTIVE_LOW.
	CS pins initialize to ACTIVE_HIGH

	pin	OPTIONS:	CS0_ActiveLow	 = Sets the CS as ACTIVE_LOW
				CS1_ActiveLow	 = Sets the CS as ACTIVE_LOW
				CS2_ActiveLow	 = Sets the CS as ACTIVE_LOW
				CS3_ActiveLow 	 = Sets the CS as ACTIVE_LOW
				CS4_ActiveLow	 = Sets the CS as ACTIVE_LOW
				


********************************************
************** Defined Macros **************

As long as the T3SPI is #included, these three macros can be used outside of the T3SPI class.
Because they are defined, you don't need to use the class pointer to use them.
*Use SPI_WRITE_16, not use t3SPI.SPI_WRITE_16  



SPI_WRITE_16(data, CTARn, CS);

*******	This macro sends a 16 bit byte.

	data	OPTIONS:	put your 16 bit variable here.

	CTARn	OPTIONS:	CTAR0	:Use the attributes that were set in CTAR0
				CTAR1	:Use the attributes that were set in CTAR1

	PCS	OPTIONS:	Any CS pin what has been enabled. (example: CS0, ALT_CS2)



SPI_WRITE_8(data, CTARn, CS);

*******	This macro sends a 8 bit byte.

	data	OPTIONS:	put your 8 bit variable here.

	CTARn	OPTIONS:	CTAR0	:Use the attributes that were set in CTAR0
				CTAR1	:Use the attributes that were set in CTAR1

	PCS	OPTIONS:	Any CS pin what has been enabled. (example: CS0, ALT_CS2)



SPI_WAIT( );

*******	This macro waits for the current transfer to finish sending.  It is useful for many
	standard speed slave devices.
