#use rs232(baud=9600, xmit=PIN_G4, rcv=PIN_G3, stream=SNSR_WATER_MAXBOTIX)// water level



float temp2Distance_MBotix[20] = {};
int sampleCounter_MBotix=0;

void sortWaterData_MBotix();
float computeDistanceX_MBotix();
void getDistance_MBotix();
