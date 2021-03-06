/******************************************************************
 NCD Relay control

 Features:
 - On/Off control
 - Supports multiple boards
 - Automatic shut off if duration specified

 Supported Attributes:
 - Switch

 http://www.github.com/rlisle/PhotonHome

 Written by Ron Lisle

 BSD license, check license.txt for more information.
 All text above must be included in any redistribution.

 Datasheets:

 Changelog:
 2019-05-18: Initial version refactored from Patriot
 ******************************************************************/

#pragma once

#include "Particle.h"
#include "device.h"

class NCD8Relay : public Device
{
 private:
    int8_t  _relayNum;
    int8_t  _boardIndex;            // Index into static arrays
    int8_t  _registerAddress;       // Is this different for different boards?
    int8_t  _duration;              // # seconds to keep on. 0 = forever
    unsigned long _stopMillis;      // time in millis to auto-stop

    bool     _isOn;                 // Remember state

    static int8_t _numControllers;    // Count of relay boards on I2C bus
    static int8_t _currentStates[];   // up to 8 relays currently supported
    static int8_t _addresses[];       // Addresses of up to 8 boards

    void    setSwitch(String value);
    String  getSwitch();

    int8_t  initialize8RelayBoard(int8_t address);
    int8_t  initializeBoard(int8_t address);
    int8_t  boardIndex(int8_t address);
    int8_t  addAddressToArray(int8_t address);

 public:
    //TODO: move duration to an HA attribute
    NCD8Relay(String name, String deviceClass, int8_t address, int8_t numRelays, int8_t relayNum, int8_t duration = 0);

    String  getConfiguration(String controllerName);

    void    setAttribute(String attribute, String value);
    String  queryAttribute(String attribute);

    void    setOn();
    void    setOff();

    void    loop();
};
