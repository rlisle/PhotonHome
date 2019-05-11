/******************************************************************
Example starting point sketch for PhotonHome library

This example uses the built-in blue LED.
You can use Home Assistant to turn the LED on or off.

It is provided as a quick start for your own sketches.
Refer also to the examples included with each PhotonHome plugin library.

http://www.github.com/rlisle/PhotonHome

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

Changelog:
2019-05-10: Initial file copied from Patriot.
******************************************************************/

#include <PHLib.h>
#include <PHLibLight.h>

PHLib *phlib;

void setup() {
    phlib = PHLib::getInstance();
    phlib->setPublishName("homeassistant");     // default anyways
    phlib->setControllerName("MyPhoton");       // Whatever you want to call it
    phlib->begin();     //TODO: combine the above 2 calls into this?

    phlib->connectMQTT("rons-mac-mini", "my-mqtt-id1"); // Something unique

    // Create devices
    //Note: D7 is not a PWM pin, so it can only turn on or off
    //      The last 4th argument specifies this.
    //      The 3rd argument can be used to invert on/off.
    // Alexa will respond to "Alexa, turn L E D on" or "Alexa, turn off L E D"
    // You can change the name 'LED' to whatever you like, but it needs to be something
    // that Alexa can recognize.
    Light *light1 = new Light(D7, "LED", false, true);

    // Tell PHLib about the devices you defined above.
    // This will send information to Home Assistant to automatically configure it.
    phlib->addDevice(light1);
}

void loop() {
    iot->loop();
}
