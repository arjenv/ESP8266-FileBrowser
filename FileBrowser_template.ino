/*********************************************************
 * 
 * Arjen Vellekoop
 * July 17, 2024
 * File Browser template
 * 
 * ESP8266
 * Template for:
 * LittleFS   File browsing
 *            sort by directories (1st) and alphabetical
 *            Download
 *            Upload
 *            Delete
 *            Rename
 *            Download directory as TAR
 * 
 *  
 ***********************************************************/

#include "FS.h"
#include <LittleFS.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>          //https://github.com/bblanchon/ArduinoJson
#include <ESPAsyncWiFiManager.h>

#define Version "FileBrowser V1.0 July 17, 2024"
#define httpPort 80

//Global variables

const char* ssid = "your_ssid";           //replace with your ssid
const char* password = "Your_password";   // replace with your password

AsyncWebServer server(80);

/***********  SETUP       ******************/
void setup() {

  Serial.begin(115200);
  server.begin();
  Serial.printf("Version: %s\n", Version);
  
  // Connect to WiFi network
  Serial.printf("\nConnecting to %s", ssid);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.printf("\nWiFi connected\n");
  Serial.printf("IP: %s\n\n", WiFi.localIP().toString().c_str());

  // Initialize LittleFS
  if(!LittleFS.begin()){
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }

  configureFiles();
  configureTAR();

  // route to index.html
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/index.html", String(), false);
  });
}

void loop () {
  // no loop  
}
