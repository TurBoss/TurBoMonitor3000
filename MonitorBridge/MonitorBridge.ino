#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>

#ifndef STASSID
#define STASSID "WLAN_1D"
#define STAPSK  "0knEr3JwIpoAY"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

unsigned int localPort = 1337;      // local port to listen on
WiFiServer server(localPort);

void setup() {

  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    delay(5000);
    ESP.restart();
  }

  // Port defaults to 8266
  ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  ArduinoOTA.setHostname("TurBoMonitor");

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  ArduinoOTA.setPasswordHash("59bf53a6ecde8c90369a65b049ea9e6e");

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_FS
      type = "filesystem";
    }
  });

  ArduinoOTA.begin();
  //start server

  server.begin();

}

WiFiClient client;
void loop() {
  
  ArduinoOTA.handle();

  if (!client.connected()) {
    // try to connect to a new client
    client = server.available();
  } else {
    // read data from the connected client
    if (client.available() > 0) {
      Serial.write(client.read());
    }
  }
  while (Serial.available())
  {
    client.write(Serial.read());
  }
}
