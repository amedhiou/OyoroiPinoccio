/**************************************************************************\
* Pinoccio Library                                                         *
* https://github.com/Pinoccio/library-pinoccio                             *
* Copyright (c) 2014, Pinoccio Inc. All rights reserved.                   *
* ------------------------------------------------------------------------ *
*  This program is free software; you can redistribute it and/or modify it *
*  under the terms of the MIT License as described in license.txt.         *
\**************************************************************************/
#include <SPI.h>
#include <Wire.h>
#include <Scout.h>
#include <GS.h>
#include <bitlash.h>
#include <lwm.h>
#include <js0n.h>

#define SKETCH_NAME "Custom"
#define SKETCH_BUILD -1
#define SKETCH_REVISION "unknown"

/*
 * This demo demonstrates the communication from the scout to the manager
 * Steps:
 * 1) Make sure you have provisioned the manager and the mote
 * 2) Make sure they are both connected to hq.pinoccio by testing led.red
 * 3) Flash this script to the manager. Don't worry - all the wifi credentials
 *    are saved in the eeprom so flashing would not overwrite those
 * 4) open the manager via `screen /dev/tty.usbmodem1451 115200`
 * 5) NOTE: you should see "saved" as the first response. This means that
 *    doCommand in setup() has no errors and is saved into memory properly.
 * 6) Go to hq.pinoccio and execute this in your SCOUT's shell:
 *    `message.scout(1, key("hello"));`
 *
 * Debugging
 * 1) Play around with toggling the LED to see if your scout is responsive
 * 2) If nothing works, turn on hq.verbose(1)
 *
 * TODO
 * 1) Figure out what to do with really long strings
 */

void setup() {
  Scout.setup(SKETCH_NAME, SKETCH_REVISION, SKETCH_BUILD);

  addBitlashFunction("noobprint", (bitlash_function) func_noobprint);
  doCommand(
      "function on.message.scout { noobprint(arg(1), key.print(arg(2))) }");
}

void loop() {
  Scout.loop();
}

numvar func_noobprint(void) {
  Serial.println("noobprint");
  Serial.println(getarg(1));
  Serial.println(getarg(2));
}

