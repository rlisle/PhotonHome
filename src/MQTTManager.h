/******************************************************************
MQTTManager.h

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
#pragma once
#include "Particle.h"
#include "MQTTParser.h"
#include "MQTT.h"


class MQTTManager
{
public:

  MQTTManager(String brokerIP, String connectID, String controllerName, MQTTParser *parser);

  void        publish(String topic, String message);
  void        loop();
  void        mqttHandler(char* topic, byte* payload, unsigned int length);
  void        log(String message);

private:
    MQTT      *_mqtt;
    MQTTParser *_parser;
    String    _connectID;
    String    _brokerIP;
    String    _controllerName;
    system_tick_t _lastMQTTtime;

    void      (*_callback)(char*,uint8_t*,unsigned int);
    void      connect();
    void      reconnectCheck();
};
