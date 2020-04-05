#include "status.pb.h"
#include <pb_common.h>
#include <pb.h>
#include <pb_encode.h>
#include <pb_decode.h>

#include "SPI.h"
#include "ILI9341_t3.h"

#define TFT_DC  9
#define TFT_CS 10

ILI9341_t3 tft = ILI9341_t3(TFT_CS, TFT_DC);


uint8_t data[256] = {'\0'};
CpuStatus cpu_status = CpuStatus_init_zero;
bool has_data;


void setup()
{
  tft.begin();
  tft.fillScreen(ILI9341_BLACK);

  Serial.begin(115200);


  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.setTextSize(1);
  tft.println(F("TurBo Monitor 3000"));
  tft.print(F("RealTime CPU & MEM Display   2020"));
  delay(1000);
  tft.fillScreen(ILI9341_BLACK);
}

void loop()
{
  if (has_data) {
    pb_istream_t stream = pb_istream_from_buffer(data, sizeof(data));
    bool status = pb_decode(&stream, CpuStatus_fields, &cpu_status);

    tft.setCursor (0, 0);
    
    if (!status) {
      tft.print("Cores: ");
      tft.println(cpu_status.cores);
      
      tft.print("Total: ");
      tft.println(cpu_status.cpuTotal);
      
      tft.print("Core 00: ");
      tft.println(cpu_status.core0);
      
      tft.print("Core 01: ");
      tft.println(cpu_status.core1);
      
      tft.print("Core 02: ");
      tft.println(cpu_status.core2);
      
      tft.print("Core 03: ");
      tft.println(cpu_status.core3);
      
      tft.print("Core 04: ");
      tft.println(cpu_status.core4);
      
      tft.print("Core 05: ");
      tft.println(cpu_status.core5);
      
      tft.print("Core 06: ");
      tft.println(cpu_status.core6);
      
      tft.print("Core 07: ");
      tft.println(cpu_status.core7);
      
      tft.print("Core 08: ");
      tft.println(cpu_status.core8);
      
      tft.print("Core 09: ");
      tft.println(cpu_status.core9);
      
      tft.print("Core 10: ");
      tft.println(cpu_status.core10);
      
      tft.print("Core 11: ");
      tft.println(cpu_status.core11);
      
      tft.print("RAM Memory: ");
      tft.println(cpu_status.mem_percent);
      
      tft.print("RAM Total: ");
      tft.println(cpu_status.mem_total);
      
      tft.print("RAM Available: ");
      tft.println(cpu_status.mem_available);
      
      tft.print("Disk: ");
      tft.println(cpu_status.disk_percent);
      
      tft.print("Network UP: ");
      tft.println(cpu_status.net_up);
      
      tft.print("Network DOWN: ");
      tft.println(cpu_status.net_dw);
    }
    else {
      tft.print("ERROR");
    }
  }
}

void serialEvent()
{
  int index = 0;
  while (Serial.available()) {

    data[index] = Serial.read();
    index++;
  }
  has_data = true;
}
