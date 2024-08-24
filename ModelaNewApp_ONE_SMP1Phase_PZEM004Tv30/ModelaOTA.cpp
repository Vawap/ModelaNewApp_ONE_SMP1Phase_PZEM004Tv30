#include "Setting.h"
#include "ModelaVariables.h"
#include "ModelaUtilityFunction.h"

#include <WiFi.h>
#include <HTTPClient.h>
#include <Update.h>

const char* updateOTA_URL = OTA_UPDATE_URL;

void updateOTA() {
  WiFiClient client;
  HTTPClient http;

  http.begin(client, updateOTA_URL);
  int httpCode = http.GET();

  if (httpCode == HTTP_CODE_OK) {
    int contentLength = http.getSize();
    bool canBegin = Update.begin(contentLength);

    if (canBegin) {
      Serial.println("Begin OTA. This may take a while...");
      blinkLED(100, "Updating...");
      size_t written = Update.writeStream(http.getStream());

      if (written == contentLength) {
        Serial.println("Written : " + String(written) + " successfully");
      } else {
        Serial.println("Written only : " + String(written) + "/" + String(contentLength) + ". Retry?");
      }

      if (Update.end()) {
        if (Update.isFinished()) {
          Serial.println("OTA update has finished!");
          ESP.restart();
        } else {
          Serial.println("OTA update not finished. Something went wrong!");
        }
      } else {
        Serial.println("Error Occurred. Error #: " + String(Update.getError()));
      }
    } else {
      Serial.println("Not enough space to begin OTA");
    }
  } else {
    Serial.println("Failed to connect to HTTP server. HTTP code: " + String(httpCode));
  }

  http.end();
}
