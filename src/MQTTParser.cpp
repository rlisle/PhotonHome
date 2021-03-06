/******************************************************************
MQTTParser

This class handles all MQTT parsing.

Topic strings are organized to simplify parsing:
    <controllerName>/<deviceID>/<attribute>/set|status

If the controllerName matches this controller, 
then each device is compared against the deviceID. 
When a match is found, the setAttribute method is called
or the status method's value returned.


In addition, it will respond to global PhotonHome messages:
    photonhome     config       Resend config data


http://www.github.com/rlisle/PhotonHome

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

Changelog:
2019-05-24: Add config query
2019-05-09: Created by refactoring from Patriot
******************************************************************/
#include "MQTTParser.h"

MQTTParser::MQTTParser(String controllerName, Devices *devices)
{
    _controllerName = controllerName;
    _devices = devices;
    configSendingDelegate = NULL;
}

// Topics are expected to be all lowercase and no spaces (converted to '-')
void MQTTParser::parseMessage(String topic, String message, MQTT *mqtt)
{
    topic.toLowerCase();
    message.toLowerCase();
    Serial.println("MQTTParser received: " + topic + ", " + message);

    if(topic.startsWith("photonhome")) {
        Serial.println("Config query received, Home Assistant must be restarting");
        if(configSendingDelegate != NULL) {
            configSendingDelegate->sendConfig();
        }
        return;
    }

    if(topic.startsWith(_controllerName) == false) {
        Serial.printlnf("Not this controller: %s",_controllerName.c_str());
        return;
    }

    int firstSlash = topic.indexOf('/');
    int secondSlash = topic.indexOf('/',firstSlash+1);
    int thirdSlash = topic.lastIndexOf('/');
    if(firstSlash == -1 
        || secondSlash == -1
        || thirdSlash == -1 
        || firstSlash == secondSlash 
        || secondSlash == thirdSlash) {
        Serial.println("MQTT message does not contain 3 slashes, so ignoring");
        return;
    }
    String deviceID = topic.substring(firstSlash+1,secondSlash);
    String attribute = topic.substring(secondSlash+1,thirdSlash);
    String setOrStatus = topic.substring(thirdSlash+1);

    Device* device = _devices->getDeviceWithName(deviceID);
    if(device == NULL) {
        Serial.println("MQTT message does not contain a known device");
        return;
    }
    Serial.printlnf("MQTT device: %s, attribute: %s, set/status: %s, msg: %s",
        deviceID.c_str(), attribute.c_str(), setOrStatus.c_str(), message.c_str());

    if(setOrStatus.equalsIgnoreCase("set")) {
        device->setAttribute(attribute,message);

    } else if(setOrStatus.equalsIgnoreCase("status")) {
        String result = device->queryAttribute(attribute);
        //TODO: what to do with the result?

    } else {
        Serial.println("MQTT messages is not 'set' or 'status'");
    }
}

