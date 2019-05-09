/******************************************************************
This is the primary class for the PhotonHome library.

It aggregates all the other classes, and provides
a common API for adding and configuring entities.

It leverages Home Assistant's MQTT Discovery to automatically
connect entities with Home Assistant over MQTT.

http://www.github.com/rlisle/PhotonHome

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

Changelog:
2019-05-08: Initial create based on Patriot library.
******************************************************************/
#include "PhotonHome.h"

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
    // be sure not to call anything that requires hardware be initialized here, put those in begin()
    _factory                = new Factory();
    _hasBegun               = false;
    _controllerName         = kDefaultControllerName;
    _mqttManager            = NULL;
    _mqttParser             = NULL;
    _startTime              = Time.now();
    _currentTime            = _startTime;
}

/**
 * Specify the controller's name
 * 
 * @param controllerName
 */
void PhotonHome::setControllerName(String name)
{
    _controllerName = name;
}

/**
 * Begin gets everything going.
 * It must be called exactly once by the sketch
 */
void PhotonHome::begin()
{
    if(_hasBegun) return;
    _hasBegun = true;

    Serial.begin(57600);

// _devices = new Devices();
// _deviceNames = new DeviceNames();
}

// MQTT 
void PhotonHome::connectMQTT(String brokerIP, String connectID)
{
    Serial.println("Connecting to MQTT on IP " + brokerIP);
    _mqttParser = new MQTTParser(_controllerName, _devices);
    _mqttManager = new MQTTManager(brokerIP, connectID, _controllerName, _mqttParser);
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
    }
    else
    {
        Serial.println("PhotonHome adding unnamed device. (Probably an input only device)");
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
