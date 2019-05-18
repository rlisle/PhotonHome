/******************************************************************
 NCD 8 Relay board control

 Up to 8 relay boards can reside on a single I2C bus.
 
 Features:
 - On/Off control
 - Supports multiple boards
 - Automatic shut off if duration specified

 Supported Attributes:
 - Switch

 http://www.github.com/rlisle/PhotonHome

 Written by Ron Lisle
 Based on NCD8Relay library.

 BSD license, check license.txt for more information.
 All text above must be included in any redistribution.

 Datasheets:

 Changelog:
 2019-05-15: Initial version refactored from Patriot
 ******************************************************************/

#include "NCD8Relay.h"

#define MILLIS_PER_SECOND 1000

int8_t NCD8Relay::_numControllers = 0;  // Count of relay boards on I2C bus
int8_t NCD8Relay::_currentStates[8];    // All relays initially off
int8_t NCD8Relay::_addresses[8];        // Addresses of up to 8 boards

/**
 * Constructor
 * @param address is the board address set by jumpers (0-7)
 * @param numRelays identifies which NCD Relay board by the number of relays on it
 * @param relayNum is the relay number on the NCD 8 Relay board (1-8)
 * @param name String name used to address the relay.
 * @param duration Optional seconds value to automatically turn off relay. 0 = no automatic turn off.
 */
NCD8Relay::NCD8Relay(String name, String deviceClass, int8_t address, int8_t numRelays, int8_t relayNum, int8_t duration = 0)
    : Device(name, deviceClass)
{
    _relayNum   = relayNum;
    // _isOn is left uninitialized to pickup state from SRAM
    _duration   = duration;
    _stopMillis = 0;

    switch(numRelays)
    {
        case 8:
        default:
            _boardIndex = initialize8RelayBoard(address);
            break;
    }
}

int8_t NCD8Relay::initialize8RelayBoard(int8_t address) {

    _registerAddress = 0x0A;    // Does this change for different boards?

    int8_t index = boardIndex(address);
    if(index < 0)
    {
        index = initializeBoard(address);
    }
    return index;
}

int8_t NCD8Relay::initializeBoard(int8_t address) {
    int retries;
    byte status;

    // Only the first relay loaded needs to initialize the I2C link
    if(!Wire.isEnabled()) {
        Wire.begin();
    }

    retries = 0;
    do {
        Wire.beginTransmission(address);
        Wire.write(0x00);                   // Select IO Direction register
        Wire.write(0x00);                   // Set all 8 to outputs
        status = Wire.endTransmission();    // ??? Write 'em, Dano

        // Note: pull-ups should have no effect on outputs, so this looks wrong.
        Wire.write(0x06);                   // ??? Select pull-up resistor register
        Wire.write(0x00);                   // ??? pull-ups disabled on all 8 outputs
        status = Wire.endTransmission();
    } while( status != 0 && retries++ < 3);

    if(status != 0) {
        Serial.println("Set address failed");
        //TODO: handle error
    }

    return addAddressToArray(address);
}

int8_t NCD8Relay::boardIndex(int8_t address) {
    for(int8_t index=0; index<_numControllers; index++) {
        if(_addresses[index] == address) {
            return index;
        }
    }
    return -1;
}

int8_t NCD8Relay::addAddressToArray(int8_t address) {
    //_currentStates[_numControllers] = 0x00; Leave as set in SRAM
    _addresses[_numControllers] = address;

    // // This looks wrong. Probably delete remainder of code here.
    // Wire.write(_registerAddress);
    // Wire.write(0x00);                      // Turn off all relays
    // byte status = Wire.endTransmission();
    // if(status != 0) {
    //     //TODO: handle any errors, retry, etc.
    //     Serial.println("Error turning off relays");
    // }
    return _numControllers++;
}

/**
 * setAttribute
 * 
 * @param attribute (lowercase "switch", "brightness", or "transition")
 * @param value String numerical value
 */
void NCD8Relay::setAttribute(String attribute, String value) {
    if(attribute == "switch") {
        setSwitch(value);
    }
}

/**
 * queryAttribute - return the current value for an attribute
 * 
 * @param attribute (lowercase "switch" or "brightness")
 */
String NCD8Relay::queryAttribute(String attribute) {
    if(attribute == "switch") {
        return getSwitch();
    }
    return "";
}

/**
 * Set switch
 * @param value String "0" to "100"
 */
void NCD8Relay::setSwitch(String value) {
    if(value == "on") {
        setOn();
    } else if(value == "off") {
        setOff();
    }
}

/**
 * Get switch
 * @return String current 0-100 percent value
 */
String NCD8Relay::getSwitch() {
    return _targetPercent == 0 ? "off" : "on";
}


/**
 * Set On
 */
void NCD8Relay::setOn() {

    if(_isOn) return;

    _isOn = true;

    if(_duration != 0)
    {
        _stopMillis = millis() + (_duration * MILLIS_PER_SECOND);
    }

    byte bitmap = 1 << _relayNum;
    NCD8Relay::_currentStates[_boardIndex] |= bitmap;            // Set relay's bit

    byte status;
    int retries = 0;
    do {
        Wire.beginTransmission(_addresses[_boardIndex]);
        Wire.write(_registerAddress);
        Wire.write(NCD8Relay::_currentStates[_boardIndex]);
        status = Wire.endTransmission();
    } while(status != 0 && retries++ < 3);

    if(status != 0) {
        //TODO: handle any errors, retry, etc.
        Serial.println("Error turning on relays");
    }
}

/**
 * Set relay off
 */
void NCD8Relay::setOff() {
    if(!_isOn) return;

    _isOn = false;
    _stopMillis = 0;

    byte bitmap = 1 << _relayNum;
    bitmap = 0xff ^ bitmap;
    NCD8Relay::_currentStates[_boardIndex] &= bitmap;

    byte status;
    int retries = 0;
    do {
        Wire.beginTransmission(_addresses[_boardIndex]);
        Wire.write(_registerAddress);
        Wire.write(NCD8Relay::_currentStates[_boardIndex]);
        byte status = Wire.endTransmission();
    } while(status != 0 && retries++ < 3);

    if(status != 0) {
        //TODO: handle any errors, retry, etc.
        Serial.println("Error turning off relays");
    }
}

/**
 * Private Methods
 */

/**
 * loop()
 */
void NCD8Relay::loop()
{
    if(_stopMillis != 0)
    {
        if(millis() >= _stopMillis)
        {
            _stopMillis = 0;
            setOff();
        }
    }
};
