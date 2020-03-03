#ifndef LEPTON_I2C
#define LEPTON_I2C

void lepton_perform_ffc();
//int lepton_temperature();
float raw2Celsius(float);

float lepton_temperature1(float);

//************************************
float ROI_temp(int, int);
//************************************
void lepton_restart();
void lepton_disable_agc();
void lepton_enable_agc();
void lepton_serial_number();
void lepton_sysfpatempk();
void lepton_ffc_manual();
void lepton_rad_info();
void lepton_ffc_auto();
void lepton_disable_tlinear_rad();
void lepton_fpa_roi();


#endif
