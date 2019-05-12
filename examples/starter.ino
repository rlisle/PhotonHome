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
    phlib->setControllerName("MyPhoton");       // Whatever you want to call it
    phlib->connectMQTT("rons-mac-mini", "my-mqtt-id1"); // Something unique

    // Create devices
    //Note: D7 is not a PWM pin, so it can only turn on or off
    //      The last 4th argument specifies this.
    //      The 3rd argument can be used to invert on/off.
    // Home Assistant will identify this entity as "LED"
    // You can change the name 'LED' to whatever you like, but it should be
    // something that Alexa can recognize (if you intend to use Alexa)
    Light *light1 = new Light(D7, "LED", false, true);

    // Tell PHLib about the devices you defined above.
    // This will send information to Home Assistant to automatically configure it.
    phlib->addDevice(light1);
}

void loop() {
    iot->loop();
}
