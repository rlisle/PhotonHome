/******************************************************************
Device Names

This class represents all the devices on a controller.
These can be Lights, Fans, etc.

http://www.github.com/rlisle/PhotonHome

Written by Ron Lisle

BSD license, check license.txt for more information.
All text above must be included in any redistribution.

Changelog:
2019-05-11: Refactor from Patriot
******************************************************************/
#include "devicenames.h"
#include "constants.h"

String globalDevicesVariable;

DeviceNames::DeviceNames() {
    // Without this method, strange error is reported and build fails
    _numDevices = 0;
    expose();
}

void DeviceNames::expose()
{
    if(!Particle.variable(kDevicesVariableName, globalDevicesVariable))
    {
      Serial.print("Error: Unable to expose ");
      Serial.print(kDevicesVariableName);
      Serial.println(" variable");
    }
}

// Returns non-zero if # devices exceeded
int DeviceNames::addDevice(String device)
{
  if (_numDevices < kMaxNumberOfDevices-1)
  {
    _devices[_numDevices++] = device;
    buildDevicesVariable();
  } else {
    return -1;
  }
  return 0;
}

bool DeviceNames::doesNameExist(String name)
{
  for(int i=0; i<_numDevices; i++)
  {
      if(_devices[i].equalsIgnoreCase(name)) {
        return true;
      }
  }
  return false;
}

void DeviceNames::buildDevicesVariable()
{
  String newVariable = "";
  for(int i=0; i<_numDevices; i++)
  {
    newVariable += _devices[i];
    if (i < _numDevices-1) {
      newVariable += ",";
    }
  }
  if(newVariable.length() < kMaxVariableStringLength) {
    globalDevicesVariable = newVariable;
  } else {
    Serial.println("Devices variable is too long. Need to extend to a 2nd variable");
  }

}
