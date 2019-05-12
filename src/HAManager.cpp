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

HAManager::HAManager(MQTTManager *mqttManager, String mqttPrefix, String discoveryPrefix, String controllerName)
{
    _mqttManager     = mqttManager;
    _mqttPrefix      = mqttPrefix;
    _discoveryPrefix = discoveryPrefix;
    _controllerName  = controllerName;
}

/**
 * Send discovery to Home Assistant via MQTT
 * 
 * <discovery_prefix>/<component=deviceType>/<node_id=controllerName>/<object_id>/config
 * payload = {"name":"<deviceID", "cmd_t":"mqttPath", "bri_cmd_t":"<mqttPath>"}

 * 
 * @param Device 
 **/
void HAManager::sendDiscovery(Device *device) {
    String topic = _discoveryPrefix 
                + "/" + device->type 
                + "/" + _controllerName
                + "/" + device->deviceID()
                + "/" + "config";

    String commonPath;
    if(_mqttPrefix != NULL) {
        commonPath = _mqttPrefix + "/";
    }
    commonPath += _controllerName + "/";
    commonPath += device->deviceID() + "/";
    String cmdPath = commonPath + "/set";
    String briCmdPath = commonPath + "/brightness/set";
    String message = "{\"name\":\"" + device->deviceID() 
                    + "\", \"cmd_t\":\"" + cmdPath
                    + "\", \"bri_cmd_t\":\"" + briCmdPath
                    + "\"}"
    _mqttManager->publish(topic,message);
}
