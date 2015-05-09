#ifndef t3spi_h
#define _t3spi_h

#include "mk20dx128.h"
#include "core_pins.h"

#define maxDataLength		256

#define MASTER				1
#define SLAVE				0

#define SPI_CLOCK_DIV2		0b0000	//24.0	MHz
#define SPI_CLOCK_DIV4		0b0001	//12.0	MHz
#define SPI_CLOCK_DIV6		0b0010	//08.0	MHz
#define SPI_CLOCK_DIV8		0b0011	//05.3	MHz
#define SPI_CLOCK_DIV16		0b0100	//03.0	MHz
#define SPI_CLOCK_DIV32		0b0101	//01.5	MHz
#define SPI_CLOCK_DIV64		0b0110	//750	KHz
#define SPI_CLOCK_DIV128	0b0111	//375	Khz

#define SPI_MODE0			0x00
#define SPI_MODE1			0x01
#define SPI_MODE2			0x02
#define SPI_MODE3			0x03

#define MSB_FIRST			0
#define LSB_FIRST			1

#define CTAR_0				0
#define CTAR_1				1
#define CTAR_SLAVE			2

#define	SCK					0x0D
#define MOSI				0x0B
#define MISO				0x0C
#define ALT_SCK				0x0E
#define ALT_MOSI			0x07
#define ALT_MISO			0x08

#define CS0					0x01
#define CS1					0x02
#define CS2					0x04
#define CS3					0x08
#define CS4					0x10
#define ALT_CS0				0x81
#define ALT_CS1				0x82
#define ALT_CS2				0x84
#define ALT_CS3				0x88

#define CS_ActiveLOW		1
#define CS_ActiveHIGH		0
#define CS0_ActiveLOW		0x00010000
#define CS1_ActiveLOW		0x00020000
#define CS2_ActiveLOW		0x00040000
#define CS3_ActiveLOW		0x00080000
#define CS4_ActiveLOW		0x00100000

#define SPI_SR_TXCTR		0x0000f000 //Mask isolating the TXCTR



#define SPI_WRITE_8(c,CTARn, PCS) \
	do { \
		while ((SPI0_SR & SPI_SR_TXCTR) >= 0x00004000); \
		SPI0_PUSHR = ((c)&0xff)   | SPI_PUSHR_CTAS(CTARn) | SPI_PUSHR_PCS(0x1f & PCS); \
	} while(0)


#define SPI_WRITE_16(w,CTARn, PCS) \
	do { \
		while ((SPI0_SR & SPI_SR_TXCTR) >= 0x00004000); \
		SPI0_PUSHR = ((w)&0xffff) | SPI_PUSHR_CTAS(CTARn) | SPI_PUSHR_PCS(0x1f & PCS); \
	} while(0)


#define SPI_WAIT() \
	while ((SPI0_SR & SPI_SR_TXCTR) != 0); \
	while (!(SPI0_SR & SPI_SR_TCF)); \
	SPI0_SR |= SPI_SR_TCF;


class T3SPI {
public:

	volatile int dataPointer;
	volatile int packetCT;
	volatile int data16;
	
	unsigned long timeStamp1;
	unsigned long timeStamp2;
	uint8_t ctar;


	T3SPI();
	
	//Functions for MASTER MODE
	static void begin_MASTER();
	void begin_MASTER(uint8_t sck, uint8_t mosi, uint8_t miso, uint8_t cs, bool activeState);
	static void setCTAR(bool CTARn, uint8_t size, uint8_t dataMode, uint8_t bo, uint8_t cdiv);
	static void enableCS(uint8_t cs, bool activeState);
	void tx8(volatile uint8_t *dataOUT,  int length, bool CTARn, uint8_t PCS);
	void tx16(volatile uint16_t *dataOUT, int length, bool CTARn, uint8_t PCS);
	void txrx8(volatile uint8_t *dataOUT, volatile uint8_t *dataIN, int length, bool CTARn, uint8_t PCS);
	void txrx16(volatile uint16_t *dataOUT, volatile uint16_t *dataIN, int length, bool CTARn, uint8_t PCS);

	//Functions for SLAVE MODE
	static void begin_SLAVE();
	static void begin_SLAVE(uint8_t sck, uint8_t mosi, uint8_t miso, uint8_t cs);
	static void setCTAR_SLAVE(uint8_t size, uint8_t dataMode);
	void rx8(volatile uint8_t *dataIN, int length);
	void rx16(volatile uint16_t *dataIN, int length);
	void rxtx8(volatile uint8_t *dataIN, volatile uint8_t *dataOUT, int length);
	void rxtx16(volatile uint16_t *dataIN, volatile uint16_t *dataOUT, int length);
	
	//Global Functions
	static void start();
	static void stop();
	static void end();
	void printStatistics(int length);

private:
	static void setMCR(bool mode);
	static void setFrameSize(uint8_t CTARn, uint8_t size);
	static void setMode(uint8_t CTARn, uint8_t dataMode);
	static void setBitOrder(bool CTARn, uint8_t bo);
	static void setClockDivider(bool CTARn, uint8_t cdiv);
	static void enablePins(uint8_t sck, uint8_t mosi, uint8_t miso, uint8_t cs, bool activeState);
	static void enablePins_SLAVE(uint8_t sck, uint8_t mosi, uint8_t miso, uint8_t cs);
	static void setCS_ActiveLOW(uint32_t);
};


//extern T3SPI_MASTER T3SPI;
#define SPIClass T3SPI //_MASTER
#define SPI T3SPI

#endif /* _t3spi_h */
