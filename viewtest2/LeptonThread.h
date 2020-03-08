#ifndef TEXTTHREAD
#define TEXTTHREAD

#include <ctime>
#include <stdint.h>

#include <QThread>
#include <QtCore>
#include <QPixmap>
#include <QImage>
#include <QString>
#include <iostream>
#include <QPainter>
#include <cstdlib>


#define PACKET_SIZE 164  //Bytes
#define PACKET_SIZE_UINT16 (PACKET_SIZE/2)   
#define NUMBER_OF_SEGMENTS 4
#define PACKETS_PER_SEGMENT 60
#define PACKETS_PER_FRAME (PACKETS_PER_SEGMENT*NUMBER_OF_SEGMENTS)
#define FRAME_SIZE_UINT16 (PACKET_SIZE_UINT16*PACKETS_PER_FRAME)
//#define FPS 27;

class LeptonThread : public QThread
{
  Q_OBJECT;

public:
  LeptonThread();
  ~LeptonThread();

  void run();

public slots:
  void performFFC();
  void snapshot();
  void restart();
  void disable_agc();
  void enable_agc();
  void serial_number();
  
  void SysFpaTempK();
  void FFC_Manual();
  void RAD_Info();
  void FFC_Auto();
  void disable_TLinear_RAD();
  void FPA_ROI();

 
  void setColorMap(int index);

signals:
  void updateText(QString);
  void updateImage(QImage);
  void updateRadiometry(QString);
  
  
private:

  QImage myImage;

  uint16_t *frameBuffer;

public:
  
  uint8_t result[PACKET_SIZE*PACKETS_PER_FRAME];

};

#endif
