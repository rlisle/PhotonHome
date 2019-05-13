/******************************************************************
MQTTParser.h

This class handles all MQTT parsing.

http://www.github.com/rlisle/PhotonHome

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

Changelog:
2019-05-09: Created by refactoring from Patriot
******************************************************************/
#pragma once
#include "devices.h"

class MQTTParser
{
public:

  MQTTParser(String mqttPrefix, String controllerName, Devices *devices);

  void parseMessage(String topic, String message);

private:
  String    _prefixString;
  Devices   *_devices;
};
