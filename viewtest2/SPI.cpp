#include "SPI.h"

int spi_cs0_fd = -1;
int spi_cs1_fd = -1;

unsigned char spi_mode = SPI_MODE_3;
unsigned char spi_bitsPerWord = 8;
unsigned int spi_speed = 200000000;

static const char *device = "/dev/spidev0.0";
uint8_t mode;
static uint8_t bits = 8;
static uint32_t speed = 32000000;

static void pabort(const char *s)
{
	perror(s);
	abort();
}

/*
	Name: spi_open
	Description: Open SPI device and configure mode
	Parameters:
		1. h : pointer int32 - return device handle
		2. spidev : string - spi device
		3. mode : uint8_t - spi mode
		4. bits : uint8_t - bits per word (normally 8)
		5. speed : uint32_t - SPI clk speed in MHz
	Returns:
		0 - on success, non-zero - fail, see function content for error return values. Alos check errno for more information.
*/
int spi_open(int *h, std::string spidev, uint8_t mode, uint8_t bits, uint32_t speed) {
	int r;
	uint8_t t8;
	uint32_t t32;
	//*h = 0; // reset device handle
	
	// open spi device
	*h = open(spidev.c_str(), O_RDWR); // open spi device for R/W
	if (*h < 0) { 
		// can't open
		*h = 0;
		return -1; // -1 for unable to open device
	}
	
	#ifdef _DEBUG_
		std::cout << "SPI.SETUP: OPEN=" << spidev << " , MODE=" << mode << " , BITS_PER_WORD=" << bits << " , SPI_CLK_SPEED_HZ=" << speed << std::endl;
	#endif
	
	// setup SPI mode
	r = ioctl(*h, SPI_IOC_WR_MODE, &mode); // set mode
	if (r < 0) {
		return -2; // -2 for unable to set SPI mode
	}
	
	r = ioctl(*h, SPI_IOC_RD_MODE, &t8); // read mode
	if (r < 0) {
		return -3;
	}
	
	if (t8 != mode) {
		fprintf(stderr, "SPI_SETUP(-4): Mode check fail. Set 0x%X but got 0x%X\n", mode, t8);
		return -4; // mode mismatch
	}
	
	#ifdef _DEBUG_
		std::cout << "SPI.SETUP: Mode set." << std::endl;
	#endif
	
	
	// set bits per word
	r = ioctl(*h, SPI_IOC_WR_BITS_PER_WORD, &bits); // set bits
	if (r < 0) {
		return -5;
	}
	
	r = ioctl(*h, SPI_IOC_RD_BITS_PER_WORD, &t8); // read bits
	if (r < 0) {
		return -6;
	}
	
	if (t8 != bits) {
		fprintf(stderr, "SPI_SETUP(-7): Bits per word check fail. Set 0x%X but got 0x%X\n", bits, t8);
		return -7; // mode mismatch
	}
	
	#ifdef _DEBUG_
		std::cout << "SPI.SETUP: Bits per word set." << std::endl;
	#endif
	
	
	// set SPI clock speed
	r = ioctl(*h, SPI_IOC_WR_MAX_SPEED_HZ, &speed); // set bits
	if (r < 0) {
		return -8;
	}
	
	r = ioctl(*h, SPI_IOC_RD_MAX_SPEED_HZ, &t32); // read bits
	if (r < 0) {
		return -9;
	}
	
	if (t32 != speed) {
		fprintf(stderr, "SPI_SETUP(-10): SPI Clock check fail. Set %d but got %d\n", speed, t32);
		return -10; // mode mismatch
	}
	
	#ifdef _DEBUG_
		std::cout << "SPI.SETUP: Clock speed set." << std::endl;
	#endif
	
	
	return 0; // OK!
}

/*
	Name: spi_close
	Description: Close SPI device
	Parameters:
		1. h : pointer int32 - opened device handle. After closing handle is reset to 0.
	Returns:
		0 - on success, non-zero - fail, see function content for error return values. Alos check errno for more information.
*/
int spi_close(int *h) {
	if (*h == 0) return 0; // closed, or alredy closed
	
	int r = close(*h);
	if (r < 0) {
		return -1; // can't close
	}
	
	#ifdef _DEBUG_
		std::cout << "SPI.CLOSE: OK." << std::endl;
	#endif
	
	*h=0; // rest device handle
	return r;
}

void OpenSPI(){
	int ret = 0;
	int fd;

	fd = open(device, O_RDWR);
	if (fd < 0)
	{
		pabort("can't open device");
	}

	ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
	if (ret == -1)
	{
		pabort("can't set spi mode");
	}

	ret = ioctl(fd, SPI_IOC_RD_MODE, &mode);
	if (ret == -1)
	{
		pabort("can't get spi mode");
	}

	ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
	if (ret == -1)
	{
		pabort("can't set bits per word");
	}

	ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
	if (ret == -1)
	{
		pabort("can't get bits per word");
	}

	ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
	if (ret == -1)
	{
		pabort("can't set max speed hz");
	}

	ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
	if (ret == -1)
	{
		pabort("can't get max speed hz");
	}
}

int SpiOpenPort (int spi_device)
{
	int status_value = -1;
	int *spi_cs_fd;

	//----- SET SPI MODE -----
	//SPI_MODE_0 (0,0)  CPOL=0 (Clock Idle low level), CPHA=0 (SDO transmit/change edge active to idle)
	//SPI_MODE_1 (0,1)  CPOL=0 (Clock Idle low level), CPHA=1 (SDO transmit/change edge idle to active)
	//SPI_MODE_2 (1,0)  CPOL=1 (Clock Idle high level), CPHA=0 (SDO transmit/change edge active to idle)
	//SPI_MODE_3 (1,1)  CPOL=1 (Clock Idle high level), CPHA=1 (SDO transmit/change edge idle to active)
	//spi_mode = SPI_MODE_3;

	//----- SET BITS PER WORD -----
	spi_bitsPerWord = 8;

	//----- SET SPI BUS SPEED -----
	spi_speed = 20000000;				//1000000 = 1MHz (1uS per bit)


	if (spi_device)
		spi_cs_fd = &spi_cs1_fd;
	else
		spi_cs_fd = &spi_cs0_fd;


	if (spi_device)
		*spi_cs_fd = open(std::string("/dev/spidev0.0").c_str(), O_RDWR);
	else
		*spi_cs_fd = open(std::string("/dev/spidev0.0").c_str(), O_RDWR);

	if (*spi_cs_fd < 0)
	{
		perror("Error - Could not open SPI device");
		exit(1);
	}

	status_value = ioctl(*spi_cs_fd, SPI_IOC_WR_MODE, &spi_mode);
	if(status_value < 0)
	{
		perror("Could not set SPIMode (WR)...ioctl fail");
		exit(1);
	}

	status_value = ioctl(*spi_cs_fd, SPI_IOC_RD_MODE, &spi_mode);
	if(status_value < 0)
	{
		perror("Could not set SPIMode (RD)...ioctl fail");
		exit(1);
	}

	status_value = ioctl(*spi_cs_fd, SPI_IOC_WR_BITS_PER_WORD, &spi_bitsPerWord);
	if(status_value < 0)
	{
		perror("Could not set SPI bitsPerWord (WR)...ioctl fail");
		exit(1);
	}

	status_value = ioctl(*spi_cs_fd, SPI_IOC_RD_BITS_PER_WORD, &spi_bitsPerWord);
	if(status_value < 0)
	{
		perror("Could not set SPI bitsPerWord(RD)...ioctl fail");
		exit(1);
	}

	status_value = ioctl(*spi_cs_fd, SPI_IOC_WR_MAX_SPEED_HZ, &spi_speed);
	if(status_value < 0)
	{
		perror("Could not set SPI speed (WR)...ioctl fail");
		exit(1);
	}

	status_value = ioctl(*spi_cs_fd, SPI_IOC_RD_MAX_SPEED_HZ, &spi_speed);
	if(status_value < 0)
	{
		perror("Could not set SPI speed (RD)...ioctl fail");
		exit(1);
	}
	return(status_value);
}

int SpiClosePort(int spi_device)
{
	int status_value = -1;
	int *spi_cs_fd;

	if (spi_device)
		spi_cs_fd = &spi_cs1_fd;
	else
		spi_cs_fd = &spi_cs0_fd;


	status_value = close(*spi_cs_fd);
	if(status_value < 0)
	{
		perror("Error - Could not close SPI device");
		exit(1);
	}
	return(status_value);
}
