#include <SI4735.h>
#include "EasyNextionLibrary.h"  // Include EasyNextionLibrary
EasyNex myNex(Serial); // Create an object of EasyNex class with the name < myNex > 
                       // Set as parameter the Hardware Serial you are going to use
#include <EEPROM.h>
//#include <Wire.h>
                    
char buffer[100] = {0};
float Rssi = 0;
float Stereo = 0;
float Snr = 0 ;
float Rds = 0;
float RdsStatus = 0;
//float volume = 0;
String STRrecu = " " ; 
#define RESET_PIN 12
char *rdsMsg;
char *stationName;
char bufferStatioName[10];
char bufferRdsMsg[60];
#define AM_FUNCTION 1
#define FM_FUNCTION 0
uint16_t frequence;

// adress arduino in memory
//int adresse = 14;   
//int 


SI4735 rx;


void setup()
{
  Serial.print("baud=115200");// Command send to the nextion to set at 115200
  myNex.begin(115200);
  Serial.begin(115200);
  while(!Serial);
  digitalWrite(RESET_PIN, HIGH);
  
  // Look for the Si47XX I2C bus address
  int16_t si4735Addr = rx.getDeviceI2CAddress(RESET_PIN);
  if ( si4735Addr == 0 ) {
    Serial.println("Si473X not found!");
    Serial.flush();
    while (1);
 } else {
   Serial.print("The SI473X / SI474X I2C address is 0x");
   Serial.println(si4735Addr, HEX);
   Serial.print("VaSi4735HEX.txt=");  
   Serial.print("\""); 
   Serial.print(HEX);
   Serial.print("\"");   
   Serial.write(0xff);
   Serial.write(0xff);
   Serial.write(0xff);    
 }
  freq_read(); // read the frequence in memory
  rx.setup(RESET_PIN, FM_FUNCTION);
  // Starts defaul radio function and band (FM; from 84 to 108 MHz; 103.9 MHz; step 100kHz)
  rx.setFM(8400, 10800, 10650, 10);
  rx.setVolume(0);
  delay(500);
  rx.setRdsConfig(1, 2, 2, 2, 2); 
  //rx.setFrequency(10140);  // Tune on 101.4 MHz = 10140
  rx.setFrequency(frequence); 
  rx.setVolume(45);
  rx.setSeekFmRssiThreshold(10);
  showStatus();
}

void freq_read() {
  frequence = EEPROM.get(14, frequence);  // Retourne la frequence contenue dans l'eeprom, à l’adresse "14"
  Serial.print("La fréquence en mémoire est = "); Serial.println(frequence/100.0, 1); 
}
 

void freq_write() {
  frequence = EEPROM.put(14, frequence);  // Ecrit la nouvelle frequence dans l'eeprom, à l’adresse "14"
  Serial.print("La fréquence mémorisée est = "); Serial.println(frequence/100.0, 1); 
}
void showFrequency( uint16_t frequence ) {
  if (rx.isCurrentTuneFM())
  {
    Serial.print("VaFreq.txt=");
    Serial.print("\""); // This is sent to the nextion display to set what object name (before the dot) and what atribute (after the dot) are you going to change.
    Serial.print(frequence /100.0, 1);  // This is the value you want to send to that object and atribute mention before.
    Serial.print("\""); 
    Serial.write(0xff);  // We always have to send this three lines after each command sent to the nextion display.
    Serial.write(0xff);
    Serial.write(0xff);
    
  }
}

void showStatus()
{
  frequence = rx.getFrequency();
  rx.getCurrentReceivedSignalQuality();
  //delay(250);
  if (rx.isCurrentTuneFM())
  {
    //delay(1500);
    Serial.print("VaFreq.txt=");
    Serial.print("\""); // This is sent to the nextion display to set what object name (before the dot) and what atribute (after the dot) are you going to change.
    Serial.print(frequence /100.0, 1);  // This is the value you want to send to that object and atribute mention before.
    Serial.print("\""); 
    Serial.write(0xff);  // We always have to send this three lines after each command sent to the nextion display.
    Serial.write(0xff);
    Serial.write(0xff);
    char stereo[6];  
    //delay(250);
    sprintf(stereo,(rx.getCurrentPilot()) ? "STEREO" : "MONO");
    Serial.print("VaStereo.txt=");
    Serial.print("\""); 
    Serial.print(stereo);
    Serial.print("\""); 
    Serial.write(0xff);
    Serial.write(0xff);
    Serial.write(0xff);
    Rssi = (rx.getCurrentRSSI());
    Serial.print("VaRssi.txt=");  // This is sent to the nextion display to set what object name (before the dot) and what atribute (after the dot) are you going to change.
    Serial.print("\""); 
    Serial.print(Rssi, 0);  // This is the value you want to send to that object and atribute mention before.
    Serial.print("\""); 
    Serial.write(0xff);  // We always have to send this three lines after each command sent to the nextion display.
    Serial.write(0xff);
    Serial.write(0xff);
    Snr = (rx.getCurrentSNR());
    Serial.print("VaSnr.txt=");  // This is sent to the nextion display to set what object name (before the dot) and what atribute (after the dot) are you going to change.
    Serial.print("\""); 
    Serial.print(Snr, 0);  // This is the value you want to send to that object and atribute mention before.
    Serial.print("\""); 
    Serial.write(0xff);  // We always have to send this three lines after each command sent to the nextion display.
    Serial.write(0xff);
    Serial.write(0xff);
    rx.setVolume(40);
    Serial.print("VaFreq.txt=");
    Serial.print("\""); // This is sent to the nextion display to set what object name (before the dot) and what atribute (after the dot) are you going to change.
    Serial.print(frequence /100.0, 1);  // This is the value you want to send to that object and atribute mention before.
    Serial.print("\""); 
    Serial.write(0xff);  // We always have to send this three lines after each command sent to the nextion display.
    Serial.write(0xff);
    Serial.write(0xff);    
       
    
  } 
}

void showStereo()
{
    char stereo[6];  
    sprintf(stereo,(rx.getCurrentPilot()) ? "STEREO" : "MONO");
    Serial.print("VaStereo.txt=");
    Serial.print("\""); 
    Serial.print(stereo);
    Serial.print("\""); 
    Serial.write(0xff);
    Serial.write(0xff);
    Serial.write(0xff);
    delay(500);
         
}

void showVolume()
{
    char volume[2];  
    sprintf(volume,(rx.getCurrentVolume()));
    Serial.print("VaVolume.txt=");
    Serial.print("\""); 
    Serial.print(volume);
    Serial.print("\""); 
    Serial.write(0xff);
    Serial.write(0xff);
    Serial.write(0xff);
    delay(500);
         
}

void showRDSMsg()
{
  if (strcmp(bufferRdsMsg, rdsMsg) == 0)
    return;   
    Serial.print("VaRdsText.txt=");  
    Serial.print("\""); 
    Serial.print(rdsMsg);
    Serial.print("\"");   
    Serial.write(0xff);
    Serial.write(0xff);
    Serial.write(0xff);
    delay(250);
}

void showRDSStation()
{
  if (strcmp(bufferStatioName, stationName) == 0)
   return;   
   Serial.print("VaRdsStation.txt=");
   Serial.print("\""); 
   Serial.print(stationName);
   Serial.print("\"");   
   Serial.write(0xff);
   Serial.write(0xff);
   Serial.write(0xff);
   delay(250);
}


void checkRDS()
{

  rx.getRdsStatus();
  if (rx.getRdsReceived())
  {
    if (rx.getRdsSync() && rx.getRdsSyncFound())
    {
      rdsMsg = rx.getRdsText2A();
      stationName = rx.getRdsText0A();
      if (rdsMsg != NULL)
        showRDSMsg();
      if (stationName != NULL)
        showRDSStation();    
    }
  }
}

void cleanRDS()
{
  Serial.print("VaRdsStation.txt=");
  Serial.print("\""); 
  Serial.print("");
  Serial.print("\"");   
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
}

/* void trigger nextion button send command

Numbers in HEX:
0   00  -   16  10  -   32  20  -   48  30
1   01  -   17  11  -   33  21  -   49  31
2   02  -   18  12  -   34  22  -   50  32
3   03  -   19  13  -   35  23  -   51  33
4   04  -   20  14  -   36  24  -   52  34
5   05  -   21  15  -   37  25  -   53  35
6   06  -   22  16  -   38  26  -   54  36
7   07  -   23  17  -   39  27  -   55  37
8   08  -   24  18  -   40  28  -   56  38
9   09  -   25  19  -   41  29  -   57  39
10  0A  -   26  1A  -   42  2A  -   58  3A
11  0B  -   27  1B  -   43  2B  -   59  3B
12  0C  -   28  1C  -   44  2C  -   60  3C
13  0D  -   29  1D  -   45  2D  -   61  3D
14  0E  -   30  1E  -   46  2E  -   62  3E
15  0F  -   31  1F  -   47  2F  -   63  3F

trigger(0) / savefreq : writefreq = printh 23 00 54 00
trigger(1) /readsavefreq : readfreq = printh 23 02 54 01 
trigger(2) / savevol = printh 23 02 54 02
trigger(3) /readsavevolume : readvol = printh 23 02 54 03
trigger(4) /volume + : volumeplus = printh 23 02 54 04
trigger(5) /volume - : volumeminus = printh 23 02 54 05
trigger(6) /muteon : mute = printh 23 02 54 06
trigger(7) /unmute = unmute = printh 23 02 54 07
trigger(8) /seek + : seekplus = printh 23 02 54 08 
trigger(9) /seek - : seekminus = printh 23 02 54 09
trigger(16) /tune + : tuneplus = printh 23 02 54 10 
trigger(17) /tune - : tuneminus = printh 23 02 54 11

*/
void trigger0(){ //savefreq
  frequence = EEPROM.put(14, frequence);  // Retourne la frequence contenue dans l'eeprom, à l’adresse "14"
  Serial.print("La fréquence mémorisée est maintenant = "); Serial.println(frequence/100.0, 1); 
  delay(250);
  cleanRDS();
  showStatus();
}
void trigger1(){ // readsave freq  
  frequence = EEPROM.get(14, frequence);  // Retourne la frequence contenue dans l'eeprom, à l’adresse "14"
  Serial.print("La fréquence qu est en mémoire est = "); Serial.println(frequence/100.0, 1); 
  delay(250);
  cleanRDS();
  showStatus();
}

void trigger2(){ // save vol  
  delay(250);
  cleanRDS();
  showStatus();
}

void trigger3(){ // readsave vol  
  delay(250);
  cleanRDS();
  showStatus();
}
 
void trigger4(){ // volume plus  
  rx.volumeUp();
  delay(250);  
}

void trigger5(){ // volume minus
  rx.volumeDown();
  delay(250);
}

void trigger6(){ // volume mute
  rx.setVolume(0);
  delay(250);
} 

void trigger7(){ // volume unmute
  rx.setVolume(40);
  delay(250);
}  


void trigger8(){ // seek plus 
  rx.seekStationProgress(showFrequency,1);
  delay(250);
  cleanRDS();
  showStatus();  
}
  
void trigger9(){ // seek minus  
  rx.seekStationProgress(showFrequency,0); 
  delay(250);
  cleanRDS();
  showStatus();
}

void trigger16(){ // tune plus  
  rx.frequencyUp();
  delay(250);
  cleanRDS();
  showStatus();
}

void trigger17(){ // tune minus
  rx.frequencyDown();
  delay(250);
  cleanRDS();
  showStatus();
}


void loop(){
  myNex.NextionListen(); // This function must be called repeatedly to response touch events
                         // from Nextion touch panel. Actually, you should place it in your loop function.
  checkRDS();
  
} 
