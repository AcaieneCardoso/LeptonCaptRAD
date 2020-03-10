#include "LeptonThread.h"
#include "Palettes.h"
#include "SPI.h"
#include "Lepton_I2C.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <wiringPi.h>

#define WIDTH 160
#define HEIGHT 120

// Debounce variables
uint8_t flag = 0;
uint16_t count = 0;

// Switch / Case variables
bool CRC_flag = true;

// Polinomio: X^16 + X^12 + X^5 + X^0 = 0x1021
static const uint16_t auiCrc16Ccitt[256] =
{
   0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
   0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
   0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,
   0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
   0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,
   0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
   0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4,
   0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,
   0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
   0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,
   0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12,
   0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
   0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,
   0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
   0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,
   0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,
   0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,
   0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
   0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,
   0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,
   0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
   0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
   0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,
   0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
   0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,
   0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3,
   0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
   0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92,
   0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,
   0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,
   0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
   0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0
};

int selectedColorMap = 0;
float  txt_temp;
int snapshotCount = 0;
int frame = 0;
static int raw [120][160];

const int* getColorMap() 
{
	if  ( selectedColorMap == 0) 
		return colormap_rainbow;
	if  ( selectedColorMap == 1) 
		return colormap_grayscale;
	if  ( selectedColorMap == 2) 
		return colormap_ironblack;
	if  ( selectedColorMap == 3) 
		return colormap_blackHot;
	if  ( selectedColorMap == 4) 
		return colormap_arctic;
	if  ( selectedColorMap == 5) 
		return colormap_blueRed;
	if  ( selectedColorMap == 6) 
		return colormap_coldest;
	if  ( selectedColorMap == 7) 
		return colormap_contrast;
	if  ( selectedColorMap == 8) 
		return colormap_doubleRainbow;
	if  ( selectedColorMap == 9) 
		return colormap_grayRed;
	if  ( selectedColorMap == 10) 
		return colormap_grayRed;
	if  ( selectedColorMap == 11) 
		return colormap_glowBow;

	return 0;
}

/*-----------------------------------------------------------------------------------------------*/
/**
 *    @brief Calculate the 16 bits CRC with SEED = 0x1021.
 *
 *    @param[in] uint16_t uiCrcInit - CRC initial value.
 *    @param[in] uint8_t* pucData - Pointer to the first byte to calculate CRC. 
 *    @param[in] uint32_t ulLen -   Number of bytes used to calculate CRC
 *                                     - To calculate: Only number of bytes of the data, NOT including
 *                                                     the 2 bytes of CRC.
 *                                     - To check: Bytes of the data + 2 bytes of CRC. CRC must be stored
 *                                                 at the last 2 positions of data buffer (BIG endian).
 *    @param[out]
 *
 *    @return uint16_t The result of CRC.
 *
 *    @note   
 *       To calculate the CRC: 
 *          The array to calculate CRC must be only the data, not including the CRC registers. The same to the 
 *          length, which must be only the number of data, NOT INCLUDING the 2 bytes of CRC.
 *
 *       To check the CRC:
 *           The last 2 bytes of the array to check the CRC is the CRC received. If the result (return)  
 *           is 0x0000, means CRC received is correct. Else, it is incorrect. CRC of the data is not modified.
 */
/*-----------------------------------------------------------------------------------------------*/
uint16_t fnCrc16BitsPoly1021(uint16_t uiCrcInit, uint8_t* pucBuffer, uint32_t ulLen)
{
   uint32_t ulCnt;
   uint16_t uiCrc;
   
   uiCrc = uiCrcInit;
   
   for (ulCnt = 0; ulCnt < ulLen; ulCnt++)
   { 
      uiCrc = (uiCrc << 8) ^ auiCrc16Ccitt[((uiCrc >> 8) ^ ((uint16_t)(*pucBuffer))) & 0x00FF];
      pucBuffer++;
   }
   
   return (uiCrc);
}

//++++++++++++++++++++++++++THREADS++++++++++++++++++++++++++

LeptonThread::LeptonThread() : QThread()
{
	//SpiOpenPort(0);
}

LeptonThread::~LeptonThread() 
{

}

//LeptonThread::lepton_button() {}

void LeptonThread::run()
{
	// Create the initial image
	QRgb red = qRgb(255,0,0);
	myImage = QImage(160, 120, QImage::Format_RGB888);
	for(int i=0;i<160;i++) {
		for(int j=0;j<120;j++) {
			myImage.setPixel(i, j, red);
		}
	}

	// Update image based on red image created above
	emit updateImage(myImage);

	// Begin the infinite loop to display SPI data on the screen
	while (true) {
		// State machine to keep sync up
		switch (CRC_flag)
​		{
    		case true: // Case when re-sync is needed
    			OpenSPI();
    			CRC_flag = false;
    			qDebug() << "Opening SPI port...";
    			break;

    		case false: // Case when communication is going well
    			// Continue reading discard packets (ID: xFxx) until new segment is available. Should be less than 10ms
				do {
					read(spi_cs0_fd, result+sizeof(uint8_t)*PACKET_SIZE*(i*PACKETS_PER_SEGMENT+j), sizeof(uint8_t)*PACKET_SIZE);
				} while( (result[sizeof(uint8_t)*PACKET_SIZE*(i*PACKETS_PER_SEGMENT+j)] & 0x0F) == 0x0F );
				
				for(int i = 0; i < NUMBER_OF_SEGMENTS; i++){
					for(int j = 0; j < PACKETS_PER_SEGMENT; j++) {
						// Read data packets from lepton over SPI
						read(spi_cs0_fd, result+sizeof(uint8_t)*PACKET_SIZE*(i*PACKETS_PER_SEGMENT+j), sizeof(uint8_t)*PACKET_SIZE);
						// Auxiliary variables
						uint8_t IDmsb, IDlsb, CRCmsb, CRClsb;
						// Keeping ID 2 bytes and CRC 2 bytes
						IDmsb = result[sizeof(uint8_t)*PACKET_SIZE*(i*PACKETS_PER_SEGMENT+j)];
						IDlsb = result[(sizeof(uint8_t)*PACKET_SIZE*(i*PACKETS_PER_SEGMENT+j)+1)];
						CRCmsb = result[(sizeof(uint8_t)*PACKET_SIZE*(i*PACKETS_PER_SEGMENT+j)+2)];
						CRClsb = result[(sizeof(uint8_t)*PACKET_SIZE*(i*PACKETS_PER_SEGMENT+j)+3)];
						// Keeping CRC 2 bytes together
						uint16_t crcLocal = ((uint16_t)(CRClsb));
						uint16_t crcLocal |= ((uint16_t)(CRCmsb)) << 8u; // isso está certo ????
						// Putting zero on 4 msbit of ID
						result[(sizeof(uint8_t)*PACKET_SIZE*(i*PACKETS_PER_SEGMENT+j)] = result[(sizeof(uint8_t)*PACKET_SIZE*(i*PACKETS_PER_SEGMENT+j)] & 0x0F;
						// Putting zero on CRC 2 bytes
						result[(sizeof(uint8_t)*PACKET_SIZE*(i*PACKETS_PER_SEGMENT+j)+2)] = 0x00;
						result[(sizeof(uint8_t)*PACKET_SIZE*(i*PACKETS_PER_SEGMENT+j)+3)] = 0x00;
						// Calculing CRC
						uint16_t crcCalc = fnCrc16BitsPoly1021(0, (result+sizeof(uint8_t)*PACKET_SIZE*(i*PACKETS_PER_SEGMENT+j)), PACKET_SIZE);
						// CRC check
						if( crcLocal == crcCalc ) // Sync is OK
						{
							// Returning ID to result
							result[sizeof(uint8_t)*PACKET_SIZE*(i*PACKETS_PER_SEGMENT+j)] = IDmsb;
							result[(sizeof(uint8_t)*PACKET_SIZE*(i*PACKETS_PER_SEGMENT+j)+1)] = IDlsb;
							// Returning CRC to result
							result[(sizeof(uint8_t)*PACKET_SIZE*(i*PACKETS_PER_SEGMENT+j)+2)] = CRCmsb;
							result[(sizeof(uint8_t)*PACKET_SIZE*(i*PACKETS_PER_SEGMENT+j)+3)] = CRClsb;
							continue;
						}
						else // Re-sync is required
						{
							qDebug() << "Error on CRC check !";
							// No need to return values to result, because it will be discarded
							CRC_flag = true;
							break;
						}
					}	
				usleep(1000/106); //??????????
				}
        		break;

            default: // Not needed because CRC_flag is initialiaze as true
        		CRC_flag = true;
		}

		frameBuffer = (uint16_t *)result;
		int row, column;
		uint16_t value;
		uint16_t minValue = 65535;
		uint16_t maxValue = 0;

		for(int i=0;i<FRAME_SIZE_UINT16;i++) {
			// Skip the first 2 uint16_t's of every packet, they're 4 header bytes
			if(i % PACKET_SIZE_UINT16 < 2) {
				continue;
			}
			
			// Flip the MSB and LSB at the last second
			int temp = result[i*2];
			result[i*2] = result[i*2+1];
			result[i*2+1] = temp;
			
			value = frameBuffer[i];
			if(value> maxValue) {
				maxValue = value;
			}
			if(value < minValue) {
				if(value != 0)
					minValue = value;
			}		
		}
	
		float diff = maxValue - minValue;
		float scale = 255/diff;
		QRgb color;
		
		for(int k=0; k<FRAME_SIZE_UINT16; k++) {
			if(k % PACKET_SIZE_UINT16 < 2) {
				continue;
			}
		
			value = (frameBuffer[k] - minValue) * scale;
			
			// Const int *colormap = colormap_medical;
			const int *colormap = getColorMap();
			color = qRgb(colormap[3*value], colormap[3*value+1], colormap[3*value+2]);
			
				if((k/PACKET_SIZE_UINT16) % 2 == 0){//1
					column = (k % PACKET_SIZE_UINT16 - 2);
					row = (k / PACKET_SIZE_UINT16)/2;
				}
				else{//2
					column = ((k % PACKET_SIZE_UINT16 - 2))+(PACKET_SIZE_UINT16-2);
					row = (k / PACKET_SIZE_UINT16)/2;
				}	
				raw[row][column] = frameBuffer[k];
								
				myImage.setPixel(column, row, color);		
		}
		
		// Find radiometric data from spot meter in 3x3 pixel box around the center of the image
		float radValue = 0;
		for(int i=0;i<3;i++){
			for(int j=0;j<3;j++){
				radValue += frameBuffer[((HEIGHT/2 + i) * PACKET_SIZE_UINT16)+ PACKET_SIZE_UINT16/2 - 1 + j];
			}
		}
		radValue = radValue / 9;
		// Radiometry values are the temperature in Kelvin * 100.
		float tempK = radValue/100;
		float tempC = tempK - 273.15;
		// float tempF = tempC * 1.8 + 32;
		
		// Update the UI with the new temperature info
		QString s;
		s.sprintf("%.2f C", tempC);
		emit updateRadiometry(s);

		// Draw crosshairs in the middle of the image
		for(int j = 0; j < 5; j++){
			myImage.setPixel(WIDTH/2-1, HEIGHT/2-3+j, 0);
			myImage.setPixel(WIDTH/2-3+j, HEIGHT/2-1, 0);
		}

		// Lets emit the signal for update
		emit updateImage(myImage);
		frame++;
	}

	wiringPiSetup();	// Setup the library
	pinMode(2, INPUT);	// Configure GPIO15 as an input
 
	//button loop with debounce treatment
    if((digitalRead(2) == 1))
    {
		if (count <= 50){
			count++;
		}
	}
	else{
		count = 0;
		flag = 0;
	}

	if((count >= 5) && (flag == 0)){
		snapshot();
		//lepton_rad_info();
		//system("/home/pi/LeptonGitOff/leptonSDKRAD/Lepton3/capture/16spd");
		flag = 1;
	}
}


void LeptonThread::snapshot()
{
	snapshotCount++;
	//---------------------- create image file -----------------------
	struct stat buf;
	const char *prefix = "rgb";
	const char *ext = ".png";
	char number[32];

	// Convert from int to string
	sprintf(number, "%d", snapshotCount);
	char name[64];

	// Appending photo name
	strcpy(name, prefix);
	strcat(name, number);
	strcat(name, ext);
	//if this name already exists
	int exists = stat(name,&buf);
	//if the name exists stat returns 0
		while(exists == 0){
			//try next number
			snapshotCount++;
			strcpy(name, prefix);
			sprintf(number, "%d", snapshotCount);
			strcat(name, number);
			strcat(name, ext);
			exists = stat(name, &buf);
		}
	//saving photo
	myImage.save(QString(name), "PNG", 100);
	
	//---------------------- create raw data text file -----------------------
	// Creating file name
	ext = ".txt";
	strcpy(name, prefix);
	strcat(name, number);
	strcat(name, ext);
	
	// Opening file
	FILE *arq = fopen(name,"wt");
	char values[64];

	for(int i = 0; i < 120; i++){
		// Except on line 0, insert a new line
		if ( i != 0 )
		{
			fputs("/n", arq);
		}
		for(int j = 0; j < 328; j++) {
			// Writing 160 pixel value + 2 ID (2 bytes) + 2 CRC (2 bytes)
			fputs(result[ j + ( i * ( PACKET_SIZE * 2 ) ) ], arq);
			// Following a ; sign
			fputs(" ; ", arq);

		}
		fputs("\n", arq);
	}
	fclose(arq);
	//---------------------- create raw data text file -----------------------
}

void LeptonThread::performFFC() {
	//perform FFC
	lepton_perform_ffc();
}

void LeptonThread::restart() {
	lepton_restart();
}

void LeptonThread::disable_agc() {
	lepton_disable_agc();
}

void LeptonThread::enable_agc() {
	lepton_enable_agc();
}

void LeptonThread::serial_number() {
	lepton_serial_number();
}

void LeptonThread::SysFpaTempK() {
	lepton_sysfpatempk();
}

void LeptonThread::FFC_Manual() {
	lepton_ffc_manual();
}

void LeptonThread::RAD_Info() {
	lepton_rad_info();
}

void LeptonThread::FFC_Auto() {
	lepton_ffc_auto();
}

void LeptonThread::disable_TLinear_RAD() {
	lepton_disable_tlinear_rad();
}

void LeptonThread::FPA_ROI() {
	lepton_fpa_roi();
}

void LeptonThread::setColorMap(int index) {
	selectedColorMap = index;
}