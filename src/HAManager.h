/******************************************************************
HAManager.h

This class handles all Home Assistant interactions.

http://www.github.com/rlisle/PhotonHome

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

Changelog:
2019-05-11: Initial file creation
******************************************************************/
#pragma once

#include "Particle.h"
#include "HAManager.h"
#include "MQTTManager.h"


class HAManager
{
public:

  HAManager(MQTTManager *mqttManager, String discoveryPrefix);

  void      discovery(Device *device);

private:
  MQTTManager *_mqttManager;
  String      _discoveryPrefix;
};
