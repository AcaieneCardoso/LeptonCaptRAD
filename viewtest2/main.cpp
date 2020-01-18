#include <QApplication>
#include <QThread>
#include <QMutex>
#include <QMessageBox>

#include <QColor>
#include <QLabel>
#include <QtDebug>
#include <QString>
#include <QPushButton>
#include <QComboBox>

#include "LeptonThread.h"
#include "MyLabel.h"


#include <iostream>
#include <wiringPi.h>


#include <stdlib.h>


int main( int argc, char **argv )
{
	int WindowWidth = 680;
	int WindowHeight = 560;
	//int WindowWidth = 640;
	//int WindowHeight = 540;
	int ImageWidth = 520;
	int ImageHeight = 440;




	//create the app
	QApplication a( argc, argv );
	
	QWidget *myWidget = new QWidget;
	myWidget->setGeometry(400, 300, WindowWidth, WindowHeight);

	//create an image placeholder for myLabel
	//fill the top left corner with red, just bcuz
	QImage myImage;
	myImage = QImage(ImageWidth, ImageHeight, QImage::Format_RGB888);
	
	//create a label, and set it's image to the placeholder
	MyLabel myLabel(myWidget);
	myLabel.setGeometry(10, 10, ImageWidth, ImageHeight);
	myLabel.setPixmap(QPixmap::fromImage(myImage));


	int numberOfButtons = 12;
	//create a FFC button
	QPushButton *button1 = new QPushButton("FFC", myWidget);
	button1->setGeometry(ImageWidth/numberOfButtons-35, WindowHeight-45, 65, 30);
	
	//create a Snapshot button
	QPushButton *button2 = new QPushButton("Capture", myWidget);
	button2->setGeometry(ImageWidth/numberOfButtons+35, WindowHeight-45, 100, 30);
	
	//create a reset button
	QPushButton *button3 = new QPushButton("Restart", myWidget);
	button3->setGeometry(ImageWidth/numberOfButtons+135, WindowHeight-45, 100, 30);

		
	//create a disable AGC button
	QPushButton *button4 = new QPushButton("Disable AGC", myWidget);
	button4->setGeometry(ImageWidth/numberOfButtons+70, WindowHeight-85, 100, 30);
	
	//enable AGC
	QPushButton *button5 = new QPushButton("Enable AGC", myWidget);
	button5->setGeometry(ImageWidth/numberOfButtons+170, WindowHeight-85, 100, 30);


	//creat enable Serial button
	QPushButton *button6 = new QPushButton("Serial", myWidget);
	button6->setGeometry(ImageWidth/numberOfButtons+270, WindowHeight-85, 100, 30);
	
	//creat GetSysFpaKelvin
	QPushButton *button7= new QPushButton("FpaTempK", myWidget);
	button7->setGeometry(ImageWidth/numberOfButtons+370, WindowHeight-85, 100, 30);

	//Change FFC to manual
	QPushButton *button8 = new QPushButton("FFC Manual", myWidget);
	button8->setGeometry(ImageWidth/numberOfButtons+470, WindowHeight-85, 100, 30);



	//create a get RAD button

	QPushButton *button9 = new QPushButton("RAD info", myWidget);
	button9->setGeometry(ImageWidth/numberOfButtons+235, WindowHeight-45, 80, 30);


	//create a FFC auto button
	QPushButton *button10 = new QPushButton("FFC Auto", myWidget);
	button10->setGeometry(ImageWidth/numberOfButtons+315, WindowHeight-45, 100, 30);
	//
	//create a disable RAD TLinear  button
	QPushButton *button11 = new QPushButton("Dis TLinear", myWidget);
	button11->setGeometry(ImageWidth/numberOfButtons+415, WindowHeight-45, 100, 30);


	//create a ROI button
	QPushButton *button12 = new QPushButton("FPA ROI", myWidget);
	button12->setGeometry(ImageWidth/numberOfButtons+515, WindowHeight-45, 100, 30);






	// Add combobox
	//QLabel *selectBoxLabel = new QLabel(myWidget);
	//selectBoxLabel->setGeometry(10, WindowHeight -65, 50, 30);
	//selectBoxLabel->setText("Color");

	QComboBox *selectBox = new QComboBox(myWidget);
	selectBox->addItem("Rainbow");
	selectBox->addItem("Gray Scale");
	selectBox->addItem("Iron Black");
	selectBox->addItem("Arctic");
	selectBox->addItem("Blue Red");
	selectBox->addItem("Coldest");
	selectBox->addItem("Contrast");
	selectBox->addItem("Double Rainbow");
	selectBox->addItem("Gray Red");
	selectBox->addItem("Glow bow");
	selectBox->addItem("Hottest");
	selectBox->setGeometry(10, WindowHeight -85, 100, 30);


	//create a thread to gather SPI data
	//when the thread emits updateImage, the label should update its image accordingly
	LeptonThread *thread = new LeptonThread();
	QObject::connect(thread, SIGNAL(updateImage(QImage)), &myLabel, SLOT(setImage(QImage)));
	
	//connect ffc button to the thread's ffc action
	QObject::connect(button1, SIGNAL(clicked()), thread, SLOT(performFFC()));
	//connect snapshot button to the thread's snapshot action
	QObject::connect(button2, SIGNAL(clicked()), thread, SLOT(snapshot()));


/*connect snapshot button to the thread's snapshot action
	QObject::connect(button2, SIGNAL(
	wiringPiSetup();        // Setup the library
	pinMode(2, INPUT);     // Configure GPIO15 as an input


 for (;;)
    {

      

		if((digitalRead(2) == 1)){
			// system("/home/pi/LeptonGitOff/leptonSDKRAD/Lepton3/capture/tt"),
			
			clicked()), thread, SLOT(snapshot()));

			//thread, SLOT(snapshot());
			//thread, snapshot();
			//printf("Pressed!\n");

		}


	//	clicked()), thread, SLOT(snapshot()));

/*
//*****************************************************************
		//GPIO BUTTON IMPLEMENTED with debounce treatment
		//pin 10 uartRX  GPIO 15 , 3V3: pin ? , GND: pin 9
		//using namespace std;

		wiringPiSetup();        // Setup the library
		pinMode(2, INPUT);     // Configure GPIO15 as an input


    for (;;)
    {

      

		if((digitalRead(2) == 1)){
			// system("/home/pi/LeptonGitOff/leptonSDKRAD/Lepton3/capture/tt"),
			thread, SLOT(snapshot());
			//thread, snapshot();
			printf("Pressed!\n");

		}
	}
			//printf("Botao Pressionado");

			
		/*{
			if (count <= 50){
				count++;
			}
		}else{
			count = 0;
			flag = 0;
		}

		if((count >= 10) && (flag == 0)){
			printf("Botao Pressionado");
			snapshot();
			flag = 1;
		}
		//*****************************************************************
*/


	//******************************************************************************************************
	//GPIO BUTTON IMPLEMENTED
	//Button
    //pin 10 uartRX  GPIO 15 // 3V3: pin  // GND: pin 9
    //using namespace std;

   // wiringPiSetup();        // Setup the library
   // pinMode(15, INPUT);     // Configure GPIO15 as an input

   // if(digitalRead(15) == 1)
   // {
   // 	snapshot();
   // }
    //******************************************************************************************************

	//connect restart button to the thread's restart action
	QObject::connect(button3, SIGNAL(clicked()), thread, SLOT(restart()));

	
	QObject::connect(selectBox, SIGNAL(currentIndexChanged(int)), thread, SLOT(setColorMap(int)));

	//connect agc button to the thread's restart action
	QObject::connect(button4, SIGNAL(clicked()), thread, SLOT(disable_agc()));

	//connect agc button to the thread's restart action
	QObject::connect(button5, SIGNAL(clicked()), thread, SLOT(enable_agc()));

	//connect serial button to the thread's serial action
	QObject::connect(button6, SIGNAL(clicked()), thread, SLOT(serial_number()));

	//connect GetSysFpa button to the thread's serial action
	QObject::connect(button7, SIGNAL(clicked()), thread, SLOT(SysFpaTempK()));


	//connect FFC button to the thread's FFC manual action
	QObject::connect(button8, SIGNAL(clicked()), thread, SLOT(FFC_Manual()));



	//connect RAD Info button to the thread's RAD info action
	QObject::connect(button9, SIGNAL(clicked()), thread, SLOT(RAD_Info()));




	//connect FFC auto button to the thread's FFC AUTO action
	QObject::connect(button10, SIGNAL(clicked()), thread, SLOT(FFC_Auto()));




	//connect disable TLinear RAD button to the thread's disable tlinear rad
	QObject::connect(button11, SIGNAL(clicked()), thread, SLOT(disable_TLinear_RAD()));



	//connect FPA ROI button to the thread's FPA ROI action
	QObject::connect(button12, SIGNAL(clicked()), thread, SLOT(FPA_ROI()));



	
	thread->start();
	
	myWidget->show();

	return a.exec();

/*
	wiringPiSetup();        // Setup the library
    pinMode(2, INPUT);     // Configure GPIO15 as an input


    for (;;)
    {

        if((digitalRead(2) == 1))
    //        system("/home/pi/LeptonGitOff/leptonSDKRAD/Lepton3/capture/tt"), 
            printf("Pressed!\n");

}
*/
}

