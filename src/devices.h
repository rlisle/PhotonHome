/******************************************************************
Devices

This class represents a collection of devices.
These can be Lights, Fans, etc.

http://www.github.com/rlisle/PhotonHome

Written by Ron Lisle

BSD license, check license.txt for more information.
All text above must be included in any redistribution.

Changelog:
2019-05-10: Initial file from Patriot
******************************************************************/
#pragma once

#include "Particle.h"
#include "device.h"

class Devices
{
public:

  Devices();

  void        loop();
  int         addDevice(Device *device);
  Device*     getDeviceByNum(int deviceNum);
  Device*     getDeviceWithName(String name);
  int         numDevices();

  bool       expose(String variableName);

private:
  uint8_t     _numDevices;
  Device**    _devices;

  bool        _isVariableExposed;
  String      *_ptrToVariable;

  void buildDeviceVariable();
};
