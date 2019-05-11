/******************************************************************
Device abstract class

This virtual class provides a common header for all devices
such as Lights, Switches, Fans, etc.

http://www.github.com/rlisle/Patriot

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
    int        _percent;

 public:
    // Note: refer to http://www.learncpp.com/cpp-tutorial/114-constructors-and-initialization-of-derived-classes/
    //       for an explanation of how derived constructor member initialization works.
    Device(String name = "", DeviceType type = DeviceType::Unknown) : _name(name), _type(type) { }

    virtual String name() { return _name; };
    virtual DeviceType type() { return _type; };

    // This method can either read the device directly, or use a value
    // set in the loop() if continuous or asynchronous polling is used.
    virtual int getPercent() { return _percent; }

    // You will need to override this if you are creating an output device
    // This is the method that should control it.
    virtual void setPercent(int percent) { _percent = percent; };

    // These are just convenience methods
    virtual bool isOn() { return _percent > 0; };
    virtual bool isOff() { return _percent == 0; };

    virtual void setOn() { setPercent(100); };
    virtual void setOff() { setPercent(0); };

    // Perform things continuously, such as fading or slewing
    virtual void loop() {};
};
