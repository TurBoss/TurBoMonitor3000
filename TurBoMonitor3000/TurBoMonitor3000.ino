#include <Wire.h>
#include <LiquidCrystal_SR_LCD3.h>
#include <MemoryFree.h>


const int PIN_LCD_STROBE         =  12;  // Out: LCD IC4094 shift-register strobe
const int PIN_LCD_DATA           =  11;  // Out: LCD IC4094 shift-register data
const int PIN_LCD_CLOCK          =  10;  // Out: LCD IC4094 shift-register clock
const int PIN_LCD_BACKLIGHT      =  5;  // Out: LCD backlight (PWM)

const int analogInPin1 = A0;  // Analog input pin that the potentiometer is attached to
//const int analogInPin2 = A1;  // Analog input pin that the potentiometer is attached to

int sensorValue=0;        // value read from the pot

int reachLeft = 0;
int reachRight = 0;



// srdata / srclock / strobe
LiquidCrystal_SR_LCD3 lcd(PIN_LCD_DATA, PIN_LCD_CLOCK, PIN_LCD_STROBE);

byte bar[8] = {
  0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b00000
};

byte emptybar[8] = {
  0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000
};

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

//these pins can not be changed 2/3 are special pins
//int encoderPin1 = 2;
//int encoderPin2 = 3;

volatile int lastEncoded = 0;
volatile long encoderValue = 0;

//long lastencoderValue = 0;

//int lastMSB = 0;
//int lastLSB = 0;

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


float PercToBar(){

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

  //lcd.setCursor(6, 1);
  //lcd.print(char(0));

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
      lcd.clear();
    }
    lastPage = 0;
    lcd.home (); // go home
    lcd.print(F(" TurBo Monitor 3000  RealTime CPU & MEM "));
    lcd.setCursor (0, 2);
    lcd.print(F(" 2014                     Display       "));
    lcd.setCursor(38,1);
    lcd.print(F("P1"));
  }
  //             PAGE 1             //
  else if (page == 1) {
    if (lastPage != 1){
      lcd.clear();
    }
    lastPage = 1;

    lcd.home (); // go home
    lcd.print(F("CORES:"));
    lcd.setCursor (6, 0);
    lcd.print(core);

    lcd.setCursor(8, 0);
    lcd.print(F("C1:"));
    lcd.setCursor (11, 0);
    lcd.print(cpu[0]);

    lcd.setCursor(16, 0);
    lcd.print(F("C2:"));
    lcd.setCursor (19, 0);
    lcd.print(cpu[1]); 

    lcd.setCursor(24, 0);
    lcd.print(F("C3:"));
    lcd.setCursor (27, 0);
    lcd.print(cpu[2]);

    lcd.setCursor(32, 0);
    lcd.print(F("C4:"));
    lcd.setCursor (35, 0);
    lcd.print(cpu[3]);

    lcd.setCursor(0, 1);
    lcd.print(F("USAGE:"));
    lcd.setCursor(6, 1);
    lcd.print(cpuTotal);

    lcd.setCursor(12, 1);
    lcd.print(F("|"));
    lcd.setCursor(33, 1);
    lcd.print(F("|"));

    int j = 0;
    for (int i = 0; i < cpuBarlines; i ++){
      lcd.setCursor(i+13,1);
      lcd.print(char(0));
      j ++;
    }
    for ( int k = 0; k < cpuEmptybarlines; k ++){
      lcd.setCursor(k+j+13,1);
      lcd.print(char(1));
    }

    lcd.setCursor(38,1);
    lcd.print(F("P2"));
  }
  //             PAGE 2             //
  else if (page == 2){
    if (lastPage != 2){
      lcd.clear();
      lastPage = 2;
    }


    lcd.setCursor(0, 0);
    lcd.print(F("MEMTOTAL:"));
    lcd.setCursor (9, 0);
    lcd.print(memTotal);

    lcd.setCursor(20, 0);
    lcd.print(F("MEMAVAIL:"));
    lcd.setCursor (29, 0);
    lcd.print(memAvailable);

    lcd.setCursor(0, 1);
    lcd.print(F("MEM:"));
    lcd.setCursor (4, 1);
    lcd.print(mem);

    lcd.setCursor(12, 1);
    lcd.print(F("|"));
    lcd.setCursor(33, 1);
    lcd.print(F("|"));

    int j = 0;
    for (int i = 0; i < memBarlines; i ++){
      lcd.setCursor(i+13,1);
      lcd.print(char(0));
      j ++;
    }
    for ( int k = 0; k < memEmptybarlines; k ++){
      lcd.setCursor(k+j+13,1);
      lcd.print(char(1));
    }

    lcd.setCursor(38,1);
    lcd.print(F("P3"));
  }
  //             PAGE 3             //
  else if (page == 3){
    if (lastPage != 3){
      lcd.clear();
      lastPage = 3;
    }
    lcd.setCursor(0,0);

    lcd.setCursor(0, 0);
    lcd.print(F("DISK:"));
    lcd.setCursor (5, 0);
    lcd.print(disk);

    lcd.setCursor(38,1);
    lcd.print(F("P4"));
  }
  //             PAGE 4             //
  else if (page == 4){
    if (lastPage != 4){
      lcd.clear();
      lastPage = 4;
    }
    lcd.setCursor(0,0);

    lcd.setCursor(0, 0);
    lcd.print(F("NETUP:"));
    lcd.setCursor (6, 0);
    lcd.print(netup);

    lcd.setCursor(0, 1);
    lcd.print(F("NETDW:"));
    lcd.setCursor (6, 1);
    lcd.print(netdw);
    
    lcd.setCursor(38,1);
    lcd.print(F("P5"));
  }
  //             PAGE 5             //
  else if (page == 5){
    if (lastPage != 5){
      lcd.clear();
      lastPage = 5;
    }
    lcd.setCursor(0,0);

    lcd.setCursor(0, 0);
    lcd.print(F("TEMP"));

    lcd.setCursor(5,0);
    lcd.print(F("CPU:"));

    lcd.setCursor(9,0);
    lcd.print(cpuTemp);

    lcd.setCursor(14,0);
    lcd.print(F("MB:"));

    lcd.setCursor(17,0);
    lcd.print(mbTemp);

    lcd.setCursor(22,0);
    lcd.print(F("GPU:"));

    lcd.setCursor(26,0);
    lcd.print(gpuTemp);

    lcd.setCursor(31,0);
    lcd.print(F("FAN:"));

    lcd.setCursor(35,0);
    lcd.print(cpuFan);


    lcd.setCursor(0,1);
    lcd.print(F("1V:"));

    lcd.setCursor(3,1);
    lcd.print(_1v);

    lcd.setCursor(9,1);
    lcd.print(F("3V:"));

    lcd.setCursor(12,1);
    lcd.print(_3v);

    lcd.setCursor(18,1);
    lcd.print(F("5V:"));

    lcd.setCursor(21,1);
    lcd.print(_5v);

    lcd.setCursor(27,1);
    lcd.print(F("12V:"));

    lcd.setCursor(31,1);
    lcd.print(_12v);


    lcd.setCursor(38,1);
    lcd.print(F("P6"));
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
/*
void CheckEncoder(){
 if (encoderValue > lastencoderValue) {
 page ++;
 if (page > 5){
 page = 5;
 }
 
 lastencoderValue = encoderValue;
 }
 else if (encoderValue < lastencoderValue){
 page --  ;
 if (page < 0){
 page = 0;
 }
 lastencoderValue = encoderValue;
 }
 //Serial.println(page);
 //Serial.println(encoderValue);
 }*/


void setup()
{
  lcd.begin (40,2);

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

  lcd.createChar(0, bar);
  lcd.createChar(1, emptybar);

  // Switch on the backlight
  /*lcd.home (); // go home
  lcd.print(F(" TurBo Monitor 3000  RealTime CPU & MEM "));
  lcd.setCursor (0, 2);
  lcd.print(F(" 2014                     Display       "));*/
  lcd.clear();
  
}

void loop()
{
  //CheckEncoder();
  CheckDir();
  StoreData();
  PercToBar();
  DisplayData();
}

void serialEvent() {
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

/*void updateEncoder(){
 int MSB = digitalRead(encoderPin1); //MSB = most significant bit
 int LSB = digitalRead(encoderPin2); //LSB = least significant bit
 
 int encoded = (MSB << 1) |LSB; //converting the 2 pin value to single number
 int sum  = (lastEncoded << 2) | encoded; //adding it to the previous encoded value
 
 if(sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderValue ++;
 if(sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoderValue --;
 
 lastEncoded = encoded; //store this value for next time
 }*/


