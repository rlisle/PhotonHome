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
#pragma once

#include "Particle.h"
#include "constants.h"
#include "MQTTManager.h"
#include "MQTTParser.h"

/**
 * Main PhotonHome object.
 * It provides an API for creating and communicating with all others.
 */
class PhotonHome {

    friend void globalMQTTHandler(char *topic, byte* payload, unsigned int length);

public:

    /**
     * Singleton pattern
     * Use getInstance instead of trying to use the constructor (private)
     * @return
     */
    static PhotonHome* getInstance();

    /**
     * Configuration methods
     * These are used to provide names and change defaults.
     * Only these methods may be called before begin()
     */
    void setPublishName(String publishName);
    void setControllerName(String controllerName);

    /**
     * begin
     * Call begin to initialize the object.
     * This is done here instead of the constructor to improve debugging.
     */
    void begin();

    /**
     * connectMQTT(byte * brokerIP, String connectID)
     * Connect to an MQTT broker with specified IP
     **/
    void connectMQTT(String brokerIP, String connectID);

    /**
     * mqttPublish(String topic, String message)
     * Publish an MQTT message on the specified topic
     **/
    void mqttPublish(String topic, String message);
    
    /**
     * Loop needs to be called periodically
     */
    void loop();

//    void addDevice(Device *device);
//    void addBehavior(Behavior *behavior);

    /**
     * Helper methods
     */

    static void log(String msg);

private:
    static PhotonHome* _instance;
    bool    _hasBegun;
    String  _controllerName;
    system_tick_t _startTime;
    system_tick_t _currentTime;


    /**
     * Constructor
     * Private because this is a singleton
     */
    PhotonHome();

    /**
     * Include other needed objects
     */
//    Devices     *_devices;
//    DeviceNames *_deviceNames;
    MQTTManager *_mqttManager;
    MQTTParser  *_mqttParser;

    void periodicReset();

    void mqttHandler(char* topic, byte* payload, unsigned int length);
};
