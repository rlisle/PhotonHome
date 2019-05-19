/******************************************************************
HAManager.cpp

This class handles all Home Assistant interactions.

http://www.github.com/rlisle/PhotonHome

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

Changelog:
2019-05-11: Initial file creation
******************************************************************/
#include "HAManager.h"
#include "constants.h"

HAManager::HAManager(MQTTManager *mqttManager, String controllerName)
{
    _mqttManager     = mqttManager;
    _controllerName  = controllerName;
}

/**
 * Send discovery to Home Assistant via MQTT
 * 
 * @param Device 
 * 
 * Refer to Home Assistant doc for MQTT discovery format
 **/
void HAManager::sendDiscovery(Device *device) {
    String message = device->getConfig(_controllerName);
    if(!message.equalsIgnoreCase("none")) {
        String topic = kDefaultDiscoveryPrefix 
                    + "/" + device->deviceClass()
                    + "/" + _controllerName
                    + "/" + device->deviceID()
                    + "/" + "config";
        _mqttManager->publish(topic,message);
    }
}
