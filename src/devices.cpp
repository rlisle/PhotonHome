/******************************************************************
Devices collection

This object manages a collection of device objects.

http://www.github.com/rlisle/PhotonHome

Written by Ron Lisle
BSD license, check license.txt for more information.
All text above must be included in any redistribution.

Changelog:
2019-05-10: Initial file from Patriot
******************************************************************/
#include "devices.h"
#include "constants.h"

Devices::Devices()
{
    // Without this method, strange error is reported and build fails
    _numDevices = 0;
    //TODO: dynamically allocate array space
    _devices = new Device *[kMaxNumberOfDevices];
}

// Returns non-zero if # devices exceeded
int Devices::addDevice(Device *device)
{
    if (_numDevices < kMaxNumberOfDevices - 1)
    {
        _devices[_numDevices++] = device;
    } else
    {
        return -1;
    }
    return 0;
}

void Devices::loop()
{
    for (int x = 0; x < _numDevices; x++)
    {
        _devices[x]->loop();
    }
}

Device *Devices::getDeviceByNum(int deviceNum)
{
    if (deviceNum < _numDevices)
    {
        return _devices[deviceNum];
    }
    return NULL;
}

Device *Devices::getDeviceWithName(String name)
{
    for (int i = 0; i < _numDevices; i++)
    {
        if (_devices[i]->name().equalsIgnoreCase(name))
        {
            return _devices[i];
        }
    }
    return NULL;
}

int Devices::numDevices()
{
    return _numDevices;
}
