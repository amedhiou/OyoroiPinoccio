
#include <SPI.h>
#include <Wire.h>
#include <Scout.h>
#include <GS.h>
#include <bitlash.h>
#include <lwm.h>
#include <js0n.h>


/*
 * Getting this to work:
 * -Make sure you have Arduino IDE 1.5
 * -Go to your arduino IDE and point the sketchbook location to THIS PROJECT
 *  ROOT where libraries and hardware resides
 * -Restart your arduino IDE
 */

/*
 * According to the new Arduino include specification, "When the user imports a
 * library into their sketch (from the "Tools > Import Library" menu), an
 * #include statement will be added for all header (.h) files in the src/
 * directory (but not its sub-folders)
 *
 * To get this to work:
 * -Run link.bash which will copy, not symlink, Oyoroi.h into the directory
 *  OyoroiPinoccio/libraries/pinoccio/src folder
 */
#include <Oyoroi.h>

#define SKETCH_NAME "Custom"
#define SKETCH_BUILD -1
#define SKETCH_REVISION "unknown"


void setup() {
  Scout.setup(SKETCH_NAME, SKETCH_REVISION, SKETCH_BUILD);
  // Add custom setup code here
}

void loop() {
  Scout.loop();
  // Add custom loop code here
  connectToServer();
}


/*
 * Create a new client but reuse the module
 */
void connectToServer() {
  GSModule *gsPtr;
  gsPtr = &pinoccio::WifiModule::instance.bp()->gs;

  Serial.println(1);

  GSTcpClient client = GSTcpClient(*gsPtr);
  IPAddress ip;
  char* url = "http://192.168.19.114";

  Serial.println(2);

  if (!gsPtr->parseIpAddress(&ip, url)) {
    ip = gsPtr->dnsLookup(url);

    if (ip == INADDR_NONE) {
      Serial.print(F("Failed to resolve "));
      Serial.println(F(", reassociating to retry"));
      return;
    }
  }

  Serial.println(3);

  if (!client.connect(ip, 8000)) {
    Serial.println(F("HQ connection failed, reassociating to retry"));
    pinoccio::WifiModule::instance.bp()->associate();
    return;
  }

  Serial.println(4);

  client.print("Hello");   // Should print a raw GET or POST request
  client.flush();

  return;
}

