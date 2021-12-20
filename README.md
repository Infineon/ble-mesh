# Bluetooth LE Mesh Library
Bluetooth LE Mesh library provides Application Programming Interfaces (APIs) for application developers to use and create Mesh Applications.

Mesh API provides a developer a simple way to incorporate BLE mesh functionality in a WICED application.
Most application will only need access to Mesh Models library. The applications that supports adding new devices to the mesh network (provisioning) and network configuration may also use Mesh Provisioning library.
Applications that use Vendor specific models in addition to standard Bluetooth SIG models, may also need to access Mesh Core library functions directly.

The LE Mesh library is available as libraries built for CM4 ARM (TM) cores using ARM, GCC tool-chains.
LE Mesh library is used in conjuction with BTStack and a porting layer such as the [one available for FreeRTOS on PSOC](https://github.com/cypresssemiconductorco/bluetooth-freertos).


## Additional Information

[Bluetooth LE Mesh Library release notes](./RELEASE.md)

For LE Mesh APIs, refer to the [API reference manual](https://infineon.github.io/ble-mesh/api_reference_manual/html/index.html).
