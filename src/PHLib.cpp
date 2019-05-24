/******************************************************************
This is the primary class for the PhotonHome library.

It aggregates all the other classes, and provides
a common API for adding and configuring entities.

It leverages Home Assistant's MQTT Discovery to automatically
connect entities with Home Assistant over MQTT.

Devices will transmit their info to Home Assistant as they are added.

http://www.github.com/rlisle/PhotonHome

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

Changelog:
2019-05-08: Initial create based on Patriot library.
******************************************************************/
#include "PHLib.h"

/**
 * Global MQTT subscribe handler
 * Called by MQTT when events are published.
 *
 * @param eventName
 * @param rawData
 */
void globalMQTTHandler(char *topic, byte* payload, unsigned int length) {
    PhotonHome* ph = PhotonHome::getInstance();
    ph->mqttHandler(topic, payload, length);
}

/**
 * Singleton PhotonHome instance
 * Use getInstance() instead of constructor
 */
PhotonHome* PhotonHome::getInstance()
{
    if(_instance == NULL)
    {
        _instance = new PhotonHome();
    }
    return _instance;
}
PhotonHome* PhotonHome::_instance = NULL;

/**
 * Helper log method
 * Simply passes msg along to Serial.println, but also provides
 * a spot to add more extensive logging or analytics
 * @param msg
 */
void PhotonHome::log(String msg)
{
    Serial.println(msg);

    PhotonHome* ph = PhotonHome::getInstance();
    if(ph->_mqttManager) {
        ph->_mqttManager->log(msg);
    }
}

/**
 * Constructor.
 */
PhotonHome::PhotonHome()
{
    _controllerName         = kDefaultControllerName;
    _mqttManager            = NULL;
    _mqttParser             = NULL;
    _haManager              = NULL;
    _startTime              = Time.now();
    _currentTime            = _startTime;

    Serial.begin(57600);

    _devices = new Devices();
    _deviceNames = new DeviceNames();
}

/**
 * Specify the controller's name
 * 
 * @param controllerName
 */
void PhotonHome::setControllerName(String name)
{
    _controllerName = name.toLowerCase();
}

// MQTT 
void PhotonHome::connectMQTT(byte *brokerIP, String connectID)
{
    Serial.println("Connecting to MQTT on IP");
    _mqttParser = new MQTTParser(_controllerName, _devices);
    _mqttManager = new MQTTManager(brokerIP, connectID, _controllerName, _mqttParser);
    _haManager = new HAManager(_mqttManager, _controllerName);
    _mqttParser->configSendingDelegate = this;

    // Note: if devices were already added, then publish them
    //sendConfig();
}

void PhotonHome::connectMQTT(String brokerDomain, String connectID)
{
    Serial.println("Connecting to MQTT on domain " + brokerDomain);
    _mqttParser = new MQTTParser(_controllerName, _devices);
    _mqttManager = new MQTTManager(brokerDomain, connectID, _controllerName, _mqttParser);
    _haManager = new HAManager(_mqttManager, _controllerName);

    // Note: if devices were added before MQTT connected, then to ahead and publish them
    sendConfig();
}

void PhotonHome::mqttPublish(String topic, String message)
{
    _mqttManager->publish(topic, message);
}

/**
 * Loop method must be called periodically,
 * typically from the sketch loop() method.
 */
void PhotonHome::loop()
{    
    _currentTime = Time.now();

    if(_devices != NULL) {
        _devices->loop();
    }

    if (_mqttManager != NULL) {
        _mqttManager->loop();
    }
}

// Add a Device
void PhotonHome::addDevice(Device *device)
{
    _devices->addDevice(device);
    if(device->name() != "") {
        Serial.println("PhotonHome adding device: "+device->name()+".");
        _deviceNames->addDevice(device->name());

        if(_haManager != NULL) {
            _haManager->sendDiscovery(device);
        }
    }
    else
    {
        Serial.println("PhotonHome adding unnamed device. (Probably an input only device)");
    }
}

// ConfigSending Protocol Implementation
void PhotonHome::sendConfig() 
{
    Serial.println("Sending config");

    if(_haManager == NULL || _devices == NULL) { 
        Serial.println("Error: required object not set");
        return; 
    }

    for(int i=0; i < _devices->numDevices(); i++) {
        Device *device = _devices->getDeviceByNum(i);
        _haManager->sendDiscovery(device);
    }
}

/******************************/
/*** MQTT Subscribe Handler ***/
/******************************/
void PhotonHome::mqttHandler(char* rawTopic, byte* payload, unsigned int length) {

    if(_mqttManager != NULL) {
        _mqttManager->mqttHandler(rawTopic, payload, length);
    }
}
