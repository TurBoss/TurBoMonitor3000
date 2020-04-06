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
      tft.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);
      tft.print("Cores: ");
      tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
      tft.print(cpu_status.cores);
      tft.println("    ");
      tft.println("");
      
      tft.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);
      tft.print("Total: ");
      tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
      tft.print(cpu_status.cpuTotal);
      tft.println("    ");
      tft.println("");
      
      tft.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);
      tft.print("Core 00: ");
      tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
      tft.print(cpu_status.core0);
      tft.println("    ");
      
      tft.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);
      tft.print("Core 01: ");
      tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
      tft.print(cpu_status.core1);
      tft.println("    ");
      
      tft.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);
      tft.print("Core 02: ");
      tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
      tft.print(cpu_status.core2);
      tft.println("    ");
      
      tft.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);
      tft.print("Core 03: ");
      tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
      tft.print(cpu_status.core3);
      tft.println("    ");
      
      tft.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);
      tft.print("Core 04: ");
      tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
      tft.print(cpu_status.core4);
      tft.println("    ");
      
      tft.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);
      tft.print("Core 05: ");
      tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
      tft.print(cpu_status.core5);
      tft.println("    ");
      
      tft.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);
      tft.print("Core 06: ");
      tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
      tft.print(cpu_status.core6);
      tft.println("    ");
      
      tft.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);
      tft.print("Core 07: ");
      tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
      tft.print(cpu_status.core7);
      tft.println("    ");
      
      tft.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);
      tft.print("Core 08: ");
      tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
      tft.print(cpu_status.core8);
      tft.println("    ");
      
      tft.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);
      tft.print("Core 09: ");
      tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
      tft.print(cpu_status.core9);
      tft.println("    ");
      
      tft.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);
      tft.print("Core 10: ");
      tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
      tft.print(cpu_status.core10);
      tft.println("    ");
      
      tft.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);
      tft.print("Core 11: ");
      tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
      tft.print(cpu_status.core11);
      tft.println("    ");
      tft.println("");
      
      tft.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);
      tft.print("RAM Memory: ");
      tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
      tft.print(cpu_status.mem_percent);
      tft.println("    ");
      
      tft.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);
      tft.print("RAM Total: ");
      tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
      tft.print(cpu_status.mem_total);
      tft.println("    ");
      
      tft.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);
      tft.print("RAM Available: ");
      tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
      tft.print(cpu_status.mem_available);
      tft.println("    ");
      tft.println("");
      
      tft.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);
      tft.print("Disk: ");
      tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
      tft.print(cpu_status.disk_percent);
      tft.println("    ");
      tft.println("");
      
      tft.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);
      tft.print("Network UP: ");
      tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
      tft.print(cpu_status.net_up);
      tft.println("    ");
      
      tft.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);
      tft.print("Network DOWN: ");
      tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
      tft.print(cpu_status.net_dw);
      tft.println("    ");
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
