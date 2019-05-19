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

// Here for reference. Now really used (yet)
enum class HomeAssistantDeviceClass {
    Alarm_Control_Panel,
    Binary_Sensor,
    Camera,
    Climate,    // HVAC
    Cover,      // Fan cover, blinds
    Fan,
    Light,
    Lock,
    Sensor,
    Switch,
    Vacuum
};

class Device {
 protected:
    String  _name;
    String  _deviceClass; // Home Assistant device class

 public:
    // Note: refer to http://www.learncpp.com/cpp-tutorial/114-constructors-and-initialization-of-derived-classes/
    //       for an explanation of how derived constructor member initialization works.
    Device(String name, String deviceClass) : _name(name), _deviceClass(deviceClass) { }

    virtual String name() { return _name; };
    virtual String deviceClass() { return _deviceClass; };

    // Helper method to format deviceID from name
    virtual String deviceID() {
        return _name.replace(" ", "-").toLowerCase();
    }

    // Handle MQTT "set" and "status" messages
    virtual void setAttribute(String attribute, String message) { }
    virtual String queryAttribute(String attribute) { return "unknown"; }

    virtual String getConfiguration(String controllerName) { return "none"; }

    // Perform things continuously, such as fading or slewing
    virtual void loop() {};

};
