/******************************************************************
Constants file

This file contains common constants used by the PhotonHome library.

http://www.github.com/rlisle/PhotonHome

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

Changelog:
2019-05-08: Initial file creation
******************************************************************/
#pragma once

/**
 * Default alive interval 900000 = 15 minutes
 * but set to 60000 = 1 minute for testing
 */
static const long kDefaultAliveInterval     = 60000;

static const int    kMaxVariableStringLength         = 266;
static const String kDevicesVariableName             = "Devices";
static const String kDefaultControllerName           = "myPhoton";
static const String kDefaultMQTTTopicPrefix          = "homeassistant";