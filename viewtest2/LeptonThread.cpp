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

//debounce variables
uint8_t flag = 0;
uint16_t count = 0;

//switch / case variables
bool CRC_flag = true;

//static const char *device = "/dev/spidev0.0";
//uint8_t mode;
//static uint8_t bits = 8;
//static uint32_t speed = 32000000;
int selectedColorMap = 0;
float  txt_temp;

//button setup
//wiringPiSetup();	// Setup the library
//pinMode(2, INPUT);	// Configure GPIO15 as an input

int snapshotCount = 0;
int frame = 0;
static int raw [120][160];

//static void pabort(const char *s)
//{
//	perror(s);
//	abort();
//}

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
		int resets = 0;
		int segmentNumber = 0;

		switch (CRC_flag)
​		{
    		case true:
    			OpenSPI();
    			CRC_flag = false;
        		break;

    		case false:
    			// Continue reading discart packets (ID: xFxx) until new segment is available. Should be less than 10ms
				do {
					read(spi_cs0_fd, result+sizeof(uint8_t)*PACKET_SIZE*(i*PACKETS_PER_SEGMENT+j), sizeof(uint8_t)*PACKET_SIZE);
				} while(result[1] == F);
				
				for(int i = 0; i < NUMBER_OF_SEGMENTS; i++){
					for(int j=0;j<PACKETS_PER_SEGMENT;j++) {
						// read data packets from lepton over SPI
						read(spi_cs0_fd, result+sizeof(uint8_t)*PACKET_SIZE*(i*PACKETS_PER_SEGMENT+j), sizeof(uint8_t)*PACKET_SIZE);
					}		
				usleep(1000/106); //??????????
				}

    			// CRC check
				if( result[(sizeof(uint8_t)*PACKET_SIZE*(i*PACKETS_PER_SEGMENT+j)+2):(sizeof(uint8_t)*PACKET_SIZE*(i*PACKETS_PER_SEGMENT+j)+3)] == fnCrc16BitsPoly1021(0, (result+sizeof(uint8_t)*PACKET_SIZE*(i*PACKETS_PER_SEGMENT+j)+4), (PACKET_SIZE-4)) )
				{
					continue;
				}
				else
				{
					qDebug() << "Error on CRC check !";
					CRC_flag = true;
				}
        		break;

            default:
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
		lepton_rad_info();
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