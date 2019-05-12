/******************************************************************
Device abstract class

This virtual class provides a common header for all devices
such as Lights, Switches, Fans, etc.

Devices are controlled using topics and messages strings.
This is optimized for MQTT, although other methods could be used.

The device name may include spaces and capitalization, but the deviceID
will have spaces converted to dashes and be forced to lower case.
For example, "Sink Light" will be converted to deviceID "sink-light"

http://www.github.com/rlisle/PhotonHome

Written by Ron Lisle

BSD license, check license.txt for more information.
All text above must be included in any redistribution.

Changelog:
2019-05-10: Initial file from Patriot
******************************************************************/
#pragma once

enum class DeviceType {
    Unknown,
    Fan,
    Light,
    Motor,
    NCD8Relay,
    Presence,
    Relay,
    Switch,
    TempHumidity,
    Ultrasonic
};

class Device {
 protected:
    String     _name;
    DeviceType _type;

 public:
    // Note: refer to http://www.learncpp.com/cpp-tutorial/114-constructors-and-initialization-of-derived-classes/
    //       for an explanation of how derived constructor member initialization works.
    Device(String name = "", DeviceType type = DeviceType::Unknown) : _name(name), _type(type) { }

    virtual String name() { return _name; };
    virtual DeviceType type() { return _type; };

    // Helper method to format deviceID from name
    virtual String deviceID() {
        return _name.replace(" ", "-").toLowerCase();
    }

    // Handle all commands and queries through the message() method
    virtual String message(String subTopic, String message) { return NULL; }

    // Perform things continuously, such as fading or slewing
    virtual void loop() {};

};
