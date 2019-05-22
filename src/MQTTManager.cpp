/******************************************************************
MQTTManager.cpp

This class handles all MQTT interactions.

Note: to avoid making this a singleton, 
the caller must provide global callback handlers (see externs).

http://www.github.com/rlisle/PhotonHome

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

Changelog:
2019-05-08: Created by refactoring from Patriot
******************************************************************/
#include "MQTTManager.h"

extern void globalMQTTHandler(char *topic, byte* payload, unsigned int length);

MQTTManager::MQTTManager(String brokerDomain, String connectID, String controllerName, MQTTParser *parser)
{
    _controllerName = controllerName;
    _parser = parser;

    _mqtt =  new MQTT((char *)brokerDomain.c_str(), 1883, globalMQTTHandler);
    connect();
}

MQTTManager::MQTTManager(byte *brokerIP, String connectID, String controllerName, MQTTParser *parser) 
{
    _controllerName = controllerName;
    _parser = parser;

    _mqtt =  new MQTT(brokerIP, 1883, globalMQTTHandler);
    connect();
}

void MQTTManager::connect() {

    _lastMQTTtime = Time.now();

    if(_mqtt == NULL) {
        log("ERROR! MQTTManager: connect called but object null");
    }

    if(_mqtt->isConnected()) {
        log("MQTT is connected, so reconnecting...");
        _mqtt->disconnect();
    }

    _mqtt->connect(_connectID);  
    if (_mqtt->isConnected()) {
        // log("MQTT is connected.");
        if(_mqtt->subscribe("#") == false) {
            log("MQTT subscribed to #");
        } else {
            log("Unable to subscribe to MQTT #");
        }
    } else {
        log("MQTT is NOT connected! Check MQTT IP address");
    }
    log("Connected at " + String(_lastMQTTtime));
}

void MQTTManager::log(String message)
{
    if(_mqtt != NULL && _mqtt->isConnected()) {
        publish("debug/" + _controllerName, message);
    } else {
        Serial.println(message);
    }
}

void MQTTManager::publish(String topic, String message, bool retain)
{
    _mqtt->publish(topic, message, retain);
}

void MQTTManager::loop()
{
    if(_mqtt != NULL && _mqtt->isConnected()) {
        _mqtt->loop();
    }

    reconnectCheck();
}

void MQTTManager::reconnectCheck() {
    system_tick_t secondsSinceLastMessage = Time.now() - _lastMQTTtime;
    if(secondsSinceLastMessage > 5 * 60) {
        log("WARNING: connection lost, reconnecting");
        log("  _lastMQTTtime = " + String(_lastMQTTtime));
        log("  Time.now() = " + String(Time.now()));
        connect();
    }
}

void MQTTManager::mqttHandler(char* rawTopic, byte* payload, unsigned int length) {

    char p[length + 1];
    memcpy(p, payload, length);
    p[length] = 0;
    String message(p);
    String topic(rawTopic);
    Serial.println("MQTTManager received topic: " + topic + ", message: " + message);

    _lastMQTTtime = Time.now();

    _parser->parseMessage(topic, message, _mqtt);
}
