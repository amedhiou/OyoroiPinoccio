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
 * -Go to your arduino IDE and point the sketchbook location to THIS PROJECT ROOT
 *  where libraries and hardware resides
 * -Restart your arduino IDE
 */

/*
 * According to the new Arduino include specification, "When the user imports a library
 * into their sketch (from the "Tools > Import Library" menu), an #include statement will
 * be added for all header (.h) files in the src/ directory (but not its sub-folders)
 *
 * To get this to work:
 * -Copy (NOT SYMLINK!!!) Wifi.h in this directory to OyoroiPinoccio/libraries/pinoccio/src folder
 */
#include <Wifi.h>

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
}


/*
 * 1) Take the client from Pinocchio
 * 2) Check if it's connected and shit
 * 3) Use that to connect to our server
 */

void connectToServer() {
  if ((pinoccio::WifiModule::instance.bp() &&
       pinoccio::WifiModule::instance.bp()->client.connected())) {

    //GSTcpClient *client;
    //*client = pinoccio::WifiModule::instance.bp()->client;

    IPAddress ip;
    char* url = "http://localhost";

    if (!gs.parseIpAddress(&ip, url)) {
      ip = gs.dnsLookup(HqHandler::host().c_str());

      if (ip == INADDR_NONE) {
        Serial.print(F("Failed to resolve "));
        Serial.print(HqHandler::host());
        Serial.println(F(", reassociating to retry"));
        return;
      }
    }

    if (!client.connect(ip, 8080)) {
      Serial.println(F("HQ connection failed, reassociating to retry"));
      associate();
      return;
    }

    client.print("Hello");
    client.flush();

    return;
  }
}

