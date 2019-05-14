/******************************************************************
MQTTParser

This class handles all MQTT parsing.

Topic strings are organized to simplify parsing:
    <controllerName>/<deviceID>/<attribute>/set|status

If the controllerName matches this controller, 
then each device is compared against the deviceID. 
When a match is found, the setAttribute method is called
or the status method's value returned.

http://www.github.com/rlisle/PhotonHome

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

Changelog:
2019-05-09: Created by refactoring from Patriot
******************************************************************/
#include "MQTTParser.h"

MQTTParser::MQTTParser(String controllerName, Devices *devices)
{
    _controllerName = controllerName;
    _devices = devices;
}

// Topics are expected to be all lowercase and no spaces (converted to '-')
void MQTTParser::parseMessage(String topic, String message, MQTT *mqtt)
{
    Serial.println("MQTTParser received: " + topic + ", " + message);

    if(topic.startsWith(_controllerName)) {
        // if(topic.length() == publishNameLength) {   // legacy
        //     int colonPosition = message.indexOf(':');
        //     String name = message.substring(0,colonPosition);
        //     String state = message.substring(colonPosition+1);
        //     // See if this is a device name. If so, update it.
        //     Device* device = _devices->getDeviceWithName(name);
        //     if(device)
        //     {
        //         int percent = state.toInt();
        //         device->setPercent(percent);
        //         return;
        //     }
        //     // If it wasn't a device name, it must be an activity.
        //     int value = state.toInt();
        //     _behaviors->performActivity(name, value);

        // } else {
            int firstSlash = topic.indexOf('/');
            int lastSlash = topic.lastIndexOf('/');
            if(firstSlash == -1 || lastSlash == -1 || firstSlash == lastSlash) {
                Serial.println("MQTT message does not contain 2 slashes, so ignoring");
                return;
            }
            String midTopic = topic.substring(firstSlash+1,lastSlash);
            String rightTopic = topic.substring(lastSlash+1);
            // Handle various topic messages
            // DEVICE
            if(midTopic.equalsIgnoreCase("device")) {
                Device* device = _devices->getDeviceWithName(rightTopic);
                if(device)
                {
                    int percent = message.toInt();
                    device->setPercent(percent);
                }

            // ACTIVITY
            } else if(midTopic.equalsIgnoreCase("activity")) {
                int value = message.toInt();
                _behaviors->performActivity(rightTopic, value);

            // PING
            } else if(midTopic.equalsIgnoreCase("ping")) {
                // Respond if ping is addressed to us
                if(rightTopic.equalsIgnoreCase(_controllerName)) {
                    //Serial.println("Ping addressed to us: "+_controllerName);
                    mqtt->publish(_publishName + "/pong/" + _controllerName, message);
                }

            // PONG
            } else if(midTopic.equalsIgnoreCase("pong")) {
                // Ignore it.

            // RESET
            } else if(midTopic.equalsIgnoreCase("reset")) {
                // Respond if reset is addressed to us
                if(rightTopic.equalsIgnoreCase(_controllerName)) {
                    Serial.println("Reset addressed to us: "+_controllerName);
                    System.reset();
                }

            // MEMORY
            } else if(midTopic.equalsIgnoreCase("memory")) {
                // Respond if memory is addressed to us
                if(rightTopic.equalsIgnoreCase(_controllerName)) {
                    //Serial.println("Memory addressed to us: "+_controllerName);
                    //Serial.println( String::format("Free memory = %d", System.freeMemory()));
                }

            // LOG
            } else if(midTopic.equalsIgnoreCase("log")) {
                // Ignore it.

            // UNKNOWN
            } else {
                //Serial.println("MQTT topic unknown");
            }
        }

    // Note: currently subscribing to _controllerName, so this will never happen
    } else if(topic.startsWith("smartthings")) {
        // Bridge may need to do something with this.
        Serial.println("MQTT smartthings received. Nothing to do.");

    }
}

