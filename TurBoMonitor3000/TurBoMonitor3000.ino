#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_SR3W.h>

#define STROBE 12
#define DATA 11
#define CLOCK 10

LiquidCrystal_SR3W lcd(DATA, CLOCK, STROBE, 3, 2, 1, 7, 6, 5, 4);

LCD *screen = &lcd;


const int analogInPin1 = A0;  // Analog input pin that the potentiometer is attached to

int sensorValue=0;        // value read from the pot

int reachLeft = 0;
int reachRight = 0;


byte bar[8] = {
  0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b00000
};

byte emptybar[8] = {
  0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000
};

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete


volatile int lastEncoded = 0;
volatile long encoderValue = 0;


String cpuTotal;
String cpu[16];
String core;
String memTotal;
String memAvailable;
String mem;
String disk;

String netup;
String netdw;

String _1v;
String _3v;
String _5v;
String _12v;
String cpuFan;
String cpuTemp;
String mbTemp;
String gpuTemp;


int page = 0;
int lastPage = 0;


int cpuBarlines = 0;
int cpuEmptybarlines = 20;


int memBarlines = 0;
int memEmptybarlines = 20;


float PercToBar()
{

  char floatbuf[32];
  cpuTotal.toCharArray(floatbuf, sizeof(floatbuf));
  float cpuTotal2 = atof(floatbuf); 

  cpuBarlines = int(float(cpuTotal2) /10 * 2);
  cpuEmptybarlines = 20 - cpuBarlines;


  char floatbuf2[32];
  mem.toCharArray(floatbuf2, sizeof(floatbuf2));
  float mem2 = atof(floatbuf2); 

  memBarlines = int(float(mem2) /10 * 2);
  memEmptybarlines = 20 - memBarlines;

  //Serial.println(barlines);
  //Serial.println(emptybarlines);

  //screen->setCursor(6, 1);
  //screen->print(char(0));

}

void StoreData()
{
  // print the string when a newline arrives:
  if (stringComplete) {

    if(inputString.indexOf("CORE") >= 0){
      core = inputString.substring(4,14);
    }

    else if(inputString.indexOf("CPU1") >= 0){
      cpu[0] = inputString.substring(5,15);
    }
    else if(inputString.indexOf("CPU2") >= 0){
      cpu[1] = inputString.substring(5,15);
    }
    else if(inputString.indexOf("CPU3") >= 0){
      cpu[2] = inputString.substring(5,15);
    }
    else if(inputString.indexOf("CPU4") >= 0){
      cpu[3] = inputString.substring(5,15);
    }
    else if(inputString.indexOf("CPU5") >= 0){
      cpu[4] = inputString.substring(5,15);
    }
    else if(inputString.indexOf("CPU6") >= 0){
      cpu[5] = inputString.substring(5,15);
    }
    else if(inputString.indexOf("CPU7") >= 0){
      cpu[6] = inputString.substring(5,15);
    }
    else if(inputString.indexOf("CPU8") >= 0){
      cpu[7] = inputString.substring(5,15);
    }

    else if(inputString.indexOf("CPUTOTAL") >= 0){
      cpuTotal = inputString.substring(8,18);
    }

    else if(inputString.indexOf("MEMTOTAL") >= 0){
      memTotal = inputString.substring(8,18);
    }
    else if(inputString.indexOf("MEMAVAIL") >= 0){
      memAvailable = inputString.substring(8,18);
    }
    else if(inputString.indexOf("MEMPERC") >= 0){
      mem = inputString.substring(7,17);
    }

    else if(inputString.indexOf("DISK") >= 0){
      disk = inputString.substring(4,14);
    }

    else if(inputString.indexOf("CPUTEMP") >= 0){
      cpuTemp = inputString.substring(7,17);
    }
    else if(inputString.indexOf("MBTEMP") >= 0){
      mbTemp = inputString.substring(6,17);
    }
    else if(inputString.indexOf("GPUTEMP") >= 0){
      gpuTemp = inputString.substring(7,17);
    }
    else if(inputString.indexOf("CPUFAN") >= 0){
      cpuFan = inputString.substring(6,16);
    }

    else if(inputString.indexOf("1V") >= 0){
      _1v = inputString.substring(2,12);
    }
    else if(inputString.indexOf("3V") >= 0){
      _3v = inputString.substring(2,12);
    }
    else if(inputString.indexOf("5V") >= 0){
      _5v = inputString.substring(2,12);
    }
    else if(inputString.indexOf("12V") >= 0){
      _12v = inputString.substring(3,13);
    }
    

    else if(inputString.indexOf("NETUP") >= 0){
      netup = inputString.substring(5);
    }
    else if(inputString.indexOf("NETDW") >= 0){
      netdw = inputString.substring(5);
    }    
    
    

    // clear the string:
    inputString = "";
    stringComplete = false;
  }
}

void DisplayData()
{ 
  //             PAGE 0             //
  if (page == 0) {
    if (lastPage != 0){
      screen->clear();
    }
    lastPage = 0;
    screen->home (); // go home
    screen->print(F(" TurBo Monitor 3000  RealTime CPU & MEM "));
    screen->setCursor (0, 2);
    screen->print(F(" 2014                     Display       "));
    screen->setCursor(38,1);
    screen->print(F("P1"));
  }
  //             PAGE 1             //
  else if (page == 1) {
    if (lastPage != 1){
      screen->clear();
    }
    lastPage = 1;

    screen->home (); // go home
    screen->print(F("CORES:"));
    screen->setCursor (6, 0);
    screen->print(core);

    screen->setCursor(8, 0);
    screen->print(F("C1:"));
    screen->setCursor (11, 0);
    screen->print(cpu[0]);

    screen->setCursor(16, 0);
    screen->print(F("C2:"));
    screen->setCursor (19, 0);
    screen->print(cpu[1]); 

    screen->setCursor(24, 0);
    screen->print(F("C3:"));
    screen->setCursor (27, 0);
    screen->print(cpu[2]);

    screen->setCursor(32, 0);
    screen->print(F("C4:"));
    screen->setCursor (35, 0);
    screen->print(cpu[3]);

    screen->setCursor(0, 1);
    screen->print(F("USAGE:"));
    screen->setCursor(6, 1);
    screen->print(cpuTotal);

    screen->setCursor(12, 1);
    screen->print(F("|"));
    screen->setCursor(33, 1);
    screen->print(F("|"));

    int j = 0;
    for (int i = 0; i < cpuBarlines; i ++){
      screen->setCursor(i+13,1);
      screen->print(char(0));
      j ++;
    }
    for ( int k = 0; k < cpuEmptybarlines; k ++){
      screen->setCursor(k+j+13,1);
      screen->print(char(1));
    }

    screen->setCursor(38,1);
    screen->print(F("P2"));
  }
  //             PAGE 2             //
  else if (page == 2){
    if (lastPage != 2){
      screen->clear();
      lastPage = 2;
    }


    screen->setCursor(0, 0);
    screen->print(F("MEMTOTAL:"));
    screen->setCursor (9, 0);
    screen->print(memTotal);

    screen->setCursor(20, 0);
    screen->print(F("MEMAVAIL:"));
    screen->setCursor (29, 0);
    screen->print(memAvailable);

    screen->setCursor(0, 1);
    screen->print(F("MEM:"));
    screen->setCursor (4, 1);
    screen->print(mem);

    screen->setCursor(12, 1);
    screen->print(F("|"));
    screen->setCursor(33, 1);
    screen->print(F("|"));

    int j = 0;
    for (int i = 0; i < memBarlines; i ++){
      screen->setCursor(i+13,1);
      screen->print(char(0));
      j ++;
    }
    for ( int k = 0; k < memEmptybarlines; k ++){
      screen->setCursor(k+j+13,1);
      screen->print(char(1));
    }

    screen->setCursor(38,1);
    screen->print(F("P3"));
  }
  //             PAGE 3             //
  else if (page == 3){
    if (lastPage != 3){
      screen->clear();
      lastPage = 3;
    }
    screen->setCursor(0,0);

    screen->setCursor(0, 0);
    screen->print(F("DISK:"));
    screen->setCursor (5, 0);
    screen->print(disk);

    screen->setCursor(38,1);
    screen->print(F("P4"));
  }
  //             PAGE 4             //
  else if (page == 4){
    if (lastPage != 4){
      screen->clear();
      lastPage = 4;
    }
    screen->setCursor(0,0);

    screen->setCursor(0, 0);
    screen->print(F("NETUP:"));
    screen->setCursor (6, 0);
    screen->print(netup);

    screen->setCursor(0, 1);
    screen->print(F("NETDW:"));
    screen->setCursor (6, 1);
    screen->print(netdw);
    
    screen->setCursor(38,1);
    screen->print(F("P5"));
  }
  //             PAGE 5             //
  else if (page == 5){
    if (lastPage != 5){
      screen->clear();
      lastPage = 5;
    }
    screen->setCursor(0,0);

    screen->setCursor(0, 0);
    screen->print(F("TEMP"));

    screen->setCursor(5,0);
    screen->print(F("CPU:"));

    screen->setCursor(9,0);
    screen->print(cpuTemp);

    screen->setCursor(14,0);
    screen->print(F("MB:"));

    screen->setCursor(17,0);
    screen->print(mbTemp);

    screen->setCursor(22,0);
    screen->print(F("GPU:"));

    screen->setCursor(26,0);
    screen->print(gpuTemp);

    screen->setCursor(31,0);
    screen->print(F("FAN:"));

    screen->setCursor(35,0);
    screen->print(cpuFan);


    screen->setCursor(0,1);
    screen->print(F("1V:"));

    screen->setCursor(3,1);
    screen->print(_1v);

    screen->setCursor(9,1);
    screen->print(F("3V:"));

    screen->setCursor(12,1);
    screen->print(_3v);

    screen->setCursor(18,1);
    screen->print(F("5V:"));

    screen->setCursor(21,1);
    screen->print(_5v);

    screen->setCursor(27,1);
    screen->print(F("12V:"));

    screen->setCursor(31,1);
    screen->print(_12v);


    screen->setCursor(38,1);
    screen->print(F("P6"));
  }
}

void CheckDir(){
  int val = analogRead(analogInPin1); 
  if (reachLeft == 0){
    if (val < 100){
      //Serial.println("Left");
      page --  ;
      if (page < 0){
        page = 0;
      }
      reachLeft = 1;
    }
  }
  if (val > 100){
    reachLeft = 0;
  }

  if (reachRight == 0){
    if (val > 800){
      //Serial.println("Right");
      page ++;
      if (page > 5){
        page = 5;
      }
      reachRight = 1;
    }
  }
  if (val < 800){
    reachRight = 0;
  }

}

void setup()
{
  screen->begin (40,2);

  Serial.begin(9600);

  // reserve 200 bytes for the inputString:
  inputString.reserve(200);

  cpuTotal.reserve(16);
  cpu[16].reserve(16);
  core.reserve(16);

  memTotal.reserve(16);
  memAvailable.reserve(16);
  mem.reserve(16);

  disk.reserve(16);

  _1v.reserve(16);
  _3v.reserve(16);
  _5v.reserve(16);
  _12v.reserve(16);

  cpuFan.reserve(16);
  cpuTemp.reserve(16);
  mbTemp.reserve(16);
  gpuTemp.reserve(16);

  //pinMode(encoderPin1, INPUT); 
  //pinMode(encoderPin2, INPUT);

  //digitalWrite(encoderPin1, HIGH); //turn pullup resistor on
  //digitalWrite(encoderPin2, HIGH); //turn pullup resistor on

  //call updateEncoder() when any high/low changed seen
  //on interrupt 0 (pin 2), or interrupt 1 (pin 3) 
  //attachInterrupt(0, updateEncoder, CHANGE); 
  //attachInterrupt(1, updateEncoder, CHANGE);

  screen->createChar(0, bar);
  screen->createChar(1, emptybar);

  // Switch on the backlight
  screen->clear();
  screen->home (); // go home
  screen->print(" TurBo Monitor 3000  RealTime CPU & MEM ");
  screen->setCursor (0, 2);
  screen->print(" 2014                     Display       ");
  delay(1000);
  screen->clear();
  
}

void loop()
{
  CheckDir();
  StoreData();
  PercToBar();
  DisplayData();
}

void serialEvent()
{
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read(); 
    // add it to the inputString:
    if (inChar == '\n') {
      stringComplete = true;
    }
    else {

      inputString += inChar;
    }
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
  }
}
