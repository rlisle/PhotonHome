# PhotonHomeLight

A PhotonHome plugin to support dimmable light devices.

This library makes it used to support dimmable lights using PWM,
or simple on/off lights if connected to a non-PWM pin.

## Usage

Include this library in any PhotonHome sketch that needs to support a light.
Refer to the example sketch included with PhotonHome.

Home Assistant will automatically detect this device via MQTT.

## Documentation

Refer to the PhotonHome Github repository and documentation for more
information.


## Contributing

Here's how you can make changes to this library and eventually contribute those changes back.

To get started, [clone the library from GitHub to your local machine](https://help.github.com/articles/cloning-a-repository/).

Change the name of the library in `library.properties` to something different. You can add your name at then end.

Modify the sources in <src> and <examples> with the new behavior.

To compile an example, use `particle compile examples/usage` command in [Particle CLI](https://docs.particle.io/guide/tools-and-features/cli#update-your-device-remotely) or use our [Desktop IDE](https://docs.particle.io/guide/tools-and-features/dev/#compiling-code).

After your changes are done you can upload them with `particle library upload` or `Upload` command in the IDE. This will create a private (only visible by you) library that you can use in other projects. Do `particle library add <myname>` to add the library to a project on your machine or add the <myname> library to a project on the Web IDE or Desktop IDE.

At this point, you can create a [GitHub pull request](https://help.github.com/articles/about-pull-requests/) with your changes to the original library.

If you wish to make your library public, use `particle library publish` or `Publish` command.

## LICENSE
Copyright 2019 Ron Lisle

Refer to the included LICENSE file.
