/******************************************************************
Device Names

This class represents all the devices in a controller.
These can be Lights, Fans, etc.
It exposes this list for use by other apps and in debugging.
This allows changing or extending the IoT network without
requiring changes to all the apps.

http://www.github.com/rlisle/PhotonHome

Written by Ron Lisle

BSD license, check license.txt for more information.
All text above must be included in any redistribution.

Datasheets:

Changelog:
2019-05-10: Initial file from Patriot
******************************************************************/
#pragma once

#include "application.h"  // Defines String
#include "constants.h"


class DeviceNames
{
public:

  DeviceNames();

  int       addDevice(String device);
  bool      doesNameExist(String name);

private:
  uint8_t   _numDevices;
  String    _devices[kMaxNumberOfDevices];

  void      expose();
  void      buildDevicesVariable();
};
