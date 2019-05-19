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

#include "PHLight.h"

// PUBLIC FUNCTIONS

/**
 * Constructor
 * @param pinNum is the pin number that is connected to the light.
 * @param name String name used to address the light.
 * @param isInverted True if On = output LOW
 * @param forceDigital True if output On/Off only (even if pin supports PWM)
 */
Light::Light(int pinNum, String name, bool isInverted, bool forceDigital)
        : Device(name, "light"),
          _pin(pinNum),
          _isInverted(isInverted),
          _forceDigital(forceDigital)
{
    _dimmingPercent           = 100;                            // Default: 100%
    _dimmingDuration          = isPwmSupported() ? 2.0 : 0;     // Default: 2 seconds
    _currentPercent           = 0.0;        // May want to leave this retained
    _targetPercent            = 0;          // May want to leave this retained
    _incrementPerMillisecond  = 0.0;
    _lastUpdateTime           = 0;
    pinMode(pinNum, OUTPUT);
    outputPWM();                        // Set initial state
}

/**
 * setAttribute
 * 
 * @param attribute (lowercase "switch", "brightness", or "transition")
 * @param value String numerical value
 */
void Light::setAttribute(String attribute, String value) {
    if(attribute == "switch") {
        setSwitch(value);
    } else if(attribute == "brightness") {
        setBrightness(value);        
    } else if(attribute == "transition") {
        setTransition(value);
    }
}

/**
 * queryAttribute - return the current value for an attribute
 * 
 * @param attribute (lowercase "switch" or "brightness")
 */
String Light::queryAttribute(String attribute) {
    if(attribute == "switch") {
        return getSwitch();
    } else if(attribute == "brightness") {
        return getBrightness();
    } else if(attribute == "transition") {
        return getTransition();
    }
    return "";
}

/*
 * Get Configuration
 * Return Home Assistant configuration for MQTT autodiscovery
 * @param controllerName String 
 * @return String Home Assistant configuration JSON
 */
String Light::getConfiguration(String controllerName) {
    String config = "{\"name\":\"" + deviceID() + "\","
                + "\"cmd_t\":\"" + controllerName + "/" + deviceID() + "/switch/set\""
                + ",\"bri_cmd_t\":\"" + controllerName + "/" + deviceID() + "/brightness/set\""
                + "}";
    return config;
}

// PRIVATE FUNCTIONS

int Light::stringToPercent(String value) {
    int percent = value.toInt();
    if(percent < 0) percent = 0;
    if(percent > 100) percent = 100;
    return percent;
}

String Light::percentToString(int value) {
    return String(value);
}

/**
 * Set switch
 * @param value String "0" to "100"
 */
void Light::setSwitch(String value) {
    if(value == "on") {
        changePercent(_dimmingPercent);
    } else if(value == "off") {
        changePercent(0);
    }
}

/**
 * Get switch
 * @return String current 0-100 percent value
 */
String Light::getSwitch() {
    return _targetPercent == 0 ? "off" : "on";
}

/**
 * Set brightness
 * @param value String "0" to "100"
 */
void Light::setBrightness(String value) {
    int percent = stringToPercent(value);
    _dimmingPercent = percent;
    changePercent(percent);
}

/**
 * Get brightness
 * @return String current 0-100 percent value
 */
String Light::getBrightness() {
    return percentToString(_currentPercent);
}

/**
 * Set transition
 * @param String milliseconds "0" to "30000"
 */
void Light::setTransition(String value) {
    _dimmingDuration = value.toFloat();
}

/**
 * Get transition
 * @return String milliseconds
 */
String Light::getTransition() {
    return String(_dimmingDuration);
}

/**
 * Change percent
 * @param percent Int new percent value
 */
void Light::changePercent(int percent) {
    if(_targetPercent == percent) return;

    _targetPercent = percent;
    if(_dimmingDuration == 0.0 || isPwmSupported() == false) {
        _currentPercent = percent;
        outputPWM();

    } else {
        startSmoothDimming();
    }
}

/**
 * Start smooth dimming
 */
void Light::startSmoothDimming() {
    if((int)_currentPercent != _targetPercent){
        _lastUpdateTime = millis();
        float delta = _targetPercent - _currentPercent;
        _incrementPerMillisecond = delta / (_dimmingDuration * 1000);
    }
}

/**
 * loop()
 */
void Light::loop()
{
    if(_currentPercent == _targetPercent) {
        // Nothing to do.
        return;
    }

    long loopTime = millis();
    float millisSinceLastUpdate = (loopTime - _lastUpdateTime);
    _currentPercent += _incrementPerMillisecond * millisSinceLastUpdate;
    if(_incrementPerMillisecond > 0) {
        if(_currentPercent > _targetPercent) {
            _currentPercent = _targetPercent;
        }
    } else {
        if(_currentPercent < _targetPercent) {
            _currentPercent = _targetPercent;
        }
    }
    _lastUpdateTime = loopTime;
    outputPWM();
};

/**
 * Set the output PWM value (0-255) based on 0-100 percent value
 */
void Light::outputPWM() {
    if(isPwmSupported()) {
        float pwm = _currentPercent;
        pwm *= 255.0;
        pwm /= 100.0;
        analogWrite(_pin, (int) pwm);
    } else {
        bool isOn = _currentPercent > 49;
        bool isHigh = (isOn && !_isInverted) || (!isOn && _isInverted);
        digitalWrite(_pin, isHigh ? HIGH : LOW);
    }
}

/**
 * Is PWM pin?
 * @param pin number
 * @return bool true if pin supports PWM
 */
bool Light::isPwmSupported()
{
    switch(_pin) {
        case D0:
        case D1:
        case D2:
        case D3:
        case A4:
        case A5:
        case A7:    // aka WKP
        case RX:
        case TX:
            return _forceDigital ? FALSE : TRUE;
        default:
            break;
    };
    return FALSE;
}