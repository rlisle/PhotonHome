/******************************************************************
 light LED dimming control

 Features:
 - On/Off control
 - Smooth dimming with duration

 Supported Attributes:
 - Switch
 - Brightness
 - Transition

 http://www.github.com/rlisle/PhotonHome

 Written by Ron Lisle

 BSD license, check license.txt for more information.
 All text above must be included in any redistribution.

 Datasheets:

 Changelog:
 2019-05-15: Initial version refactored from Patriot
 ******************************************************************/

#pragma once

#include "Particle.h"
#include "device.h"

class Light : public Device
{
 private:
    int       _pin;
    int       _dimmingPercent;
    float     _dimmingDuration;
    float     _currentPercent;
    int       _targetPercent;
    float     _incrementPerMillisecond;
    long      _lastUpdateTime;
    bool      _isInverted;                // On state = LOW instead of default HIGH
    bool      _forceDigital;              // On/Off only, even if PWM supported

    void      changePercent(int percent);
    void      startSmoothDimming();
    void      outputPWM();
    bool      isPwmSupported();
    int       stringToPercent(String value);
    String    percentToString(int value);

    void      setSwitch(String value);
    String    getSwitch();
    void      setBrightness(String percent);
    String    getBrightness();
    void      setTransition(String milliseconds);
    String    getTransition();

 public:
    Light(int pin, String name, bool isInverted=false, bool forceDigital=false);

    String    getConfiguration(String controllerName);
    
    void      setAttribute(String attribute, String value);
    String    queryAttribute(String attribute);

    void      loop();
};
