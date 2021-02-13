#include <WiFi.h>
#include <WebServer.h>
#include "index.h"

#define LILYGO_WATCH_2020_V1
#include <LilyGoWatch.h>

// use watch to control screen
TTGOClass *watch;
TFT_eSPI *tft;

// backlight on / off = 0 / 1
auto bl = 0;

// wake display
void displayOn() {
  watch->openBL();
  watch->displayWakeup();
  bl = 1;
}

// sleep display
void displayOff() {
  watch->closeBL();
  watch->displaySleep();
  bl = 0;
}

const char* ssid = "YOUR WIFI";
const char* password = "YOUR WIFI PW";

WebServer server(80);

// route handling functions
void handleRoot() {
  String s = webpage;
  server.send(200, "text/html", s);
}

void getStatus() {
  server.send(200, "text/plain", bl ? "On" : "Off");
}

void setScreen() {
  String n = server.arg("screen");
  if (n == "0") {
    displayOff();
  } else {
    displayOn();
  }
  // either send var from js or something
  server.send(200, "text/plain", bl ? "On" : "Off");
}

void setText() {
  // write on screen
  tft->fillScreen(TFT_BLACK);
  tft->setCursor(10, 100);
  String s = server.arg("text");
  Serial.println(s);
  tft->print(s);
  server.send(200);
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}


/*************************************************************************************/
void setup(void) {

  Serial.begin(115200);

  // Get TTGOClass instance
  watch = TTGOClass::getWatch();
  watch->begin();
  displayOff();

  // config text out
  tft = watch->tft;
  tft->setTextColor(TFT_CYAN);
  //tft->setCursor(10, 100);
  tft->setTextSize(2);

  Serial.println("");
  WiFi.begin(ssid, password);
  Serial.println("");

  // cancel if failed WiFi
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi failed.");
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
    return;
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // routes
  server.on("/", handleRoot);
  server.on("/get_status", getStatus);
  server.on("/set_screen", setScreen);
  server.on("/set_text", setText);

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
}
