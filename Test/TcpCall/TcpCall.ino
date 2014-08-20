
#include <SPI.h>
#include <Wire.h>
#include <Scout.h>
#include <GS.h>
#include <bitlash.h>
#include <lwm.h>
#include <js0n.h>


/*
 * According to the new Arduino include specification, "When the user imports a
 * library into their sketch (from the "Tools > Import Library" menu), an
 * #include statement will be added for all header (.h) files in the src/
 * directory (but not its sub-folders)
 */
#include <Oyoroi.h>

/*
 * Setup
 * -Make sure you have Arduino IDE 1.5
 * -Go to your arduino IDE and point the sketchbook location to THIS PROJECT
 *  ROOT where libraries and hardware resides
 * -Restart your arduino IDE
 *
 * Running:
 * -Run link.bash which will copy, not symlink, Oyoroi.h into the directory
 *  OyoroiPinoccio/libraries/pinoccio/src folder
 * -Run `python -m SimpleHTTPServer` in another terminal session as a web server
 */
#define SERVER_LOCATION "10.106.57.36"


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

  delay(3000);    // This is probably bad
}


/*
 * Create a new client but reuse the module
 */
void connectToServer() {
  GSModule *gsPtr;
  gsPtr = &pinoccio::WifiModule::instance.bp()->gs;

  GSTcpClient client = GSTcpClient(*gsPtr);
  IPAddress ip;
  char* url = SERVER_LOCATION;

  if (!gsPtr->parseIpAddress(&ip, url)) {
    ip = gsPtr->dnsLookup(url);

    if (ip == INADDR_NONE) {
      Serial.print(F("Failed to resolve "));
      Serial.print(url);
      Serial.println(F(", reassociating to retry"));
      return;
    }
  }

  if (!client.connect(ip, 8000)) {
    Serial.println(F("HQ connection failed, reassociating to retry"));
    pinoccio::WifiModule::instance.bp()->associate();
    return;
  }

  client.print("Hello");   // Should print a raw GET or POST request
  client.flush();

  return;
}

