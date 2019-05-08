# PhotonHome
PhotonHome is an open source, hobbyist maker project to assist building
Particle.io Photon based IoT devices for use with Home Assistant.

<img src="https://www.lucidchart.com/publicSegments/view/e2fb9ca3-c413-498b-b193-ab9dd6626c3a/image.png" alt="Patriot IoT Diagram" style="width: 640px;"/>

It is intended to help other hobbyists quickly create Internet-of-Things
projects that can be controlled using Home Assistant.

Home Assistant supports a vast array of IoT technologies including smart phones such as iPhone,
voice input devices such as Alexa, and can be run on inexpensive hardware such as Raspberry Pi.

This project was originally called Patriot and provided its own custom Alexa and iOS apps. But there has been such phenominal growth with Home Assistant that it would be utter nonsense to not embrace that effort.

This project consists primarily of a published particle.io Photon
library (PHlib) and examples.

Your Photon based IoT projects can be created using any of the 
standard particle.io development tools (Web IDE,
Particle IDE, or Particle CLI). PHLib handles interfacing with Home Assistant
over MQTT.

## Devices
Particle.io Photons are used to due to their ease of use and great
support. I've created low cost printed circuit boards (PCBs) to make
it even easier to use. These have wireless built-in, and can be programmed
over-the-air (OTA) so all they need is power to get started.

I've provided a set of source code files and instructions
that can be used to simplify
programming IoT devices using Particle's $19 Photon.

I've also included the Eagle files and instructions for creating
general purpose IoT printed circuit boards. Using these files,
PCBs can be ordered from China for about $1.50 each.

Note that ESPHome provides a lower cost solution, using the less expensive ESP8266.

## Release History
Refer to the (Releases page)[https://github.com/rlisle/PhotonHome/releases]
for release history.
