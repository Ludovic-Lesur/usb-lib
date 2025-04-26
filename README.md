# Description

This repository contains a cross-platform USB stack for embedded projects.

| Class | Sub-class | Host | Device |
|:---:|:---:|:---:|:---:|
| **Control pipe** | | :x: | :hammer: |
| **CDC** | **ACM** | :x: | :hammer: |

# Dependencies

The driver relies on:

* An external `types.h` header file defining the **standard C types** of the targeted MCU.
* The **embedded utility functions** defined in the [embedded-utils](https://github.com/Ludovic-Lesur/embedded-utils) repository.

Here is the versions compatibility table:

| **usb-lib** | **embedded-utils** |
|:---:|:---:|
| [sw1.1](https://github.com/Ludovic-Lesur/usb-lib/releases/tag/sw1.1) | >= [sw7.0](https://github.com/Ludovic-Lesur/embedded-utils/releases/tag/sw7.0) |
| [sw1.0](https://github.com/Ludovic-Lesur/usb-lib/releases/tag/sw1.0) | >= [sw7.0](https://github.com/Ludovic-Lesur/embedded-utils/releases/tag/sw7.0) |

# Compilation flags

| **Flag name** | **Value** | **Description** |
|:---:|:---:|:---:|
| `USB_LIB_DISABLE_FLAGS_FILE` | `defined` / `undefined` | Disable the `usb_lib_flags.h` header file inclusion when compilation flags are given in the project settings or by command line. |
| `USB_LIB_DISABLE` | `defined` / `undefined` | Disable the USB library. |
| `USB_LIB_HW_INTERFACE_ERROR_BASE_LAST` | `defined` / `undefined` | Last error base of the low level USB driver. |
| `USBD_CDC` | `defined` / `undefined` | Enable the CDC device class if defined. |
| `USBD_X_INTERFACE_INDEX` | `<value>` | Index of the device interface X. |
| `USBD_X_INTERFACE_STRING_DESCRIPTOR_INDEX` | `<value>` | Index of the string descriptor of the device interface X. |
| `USBD_X_ENDPOINT_NUMBER` | `<value>` | Endpoint number assigned to the device interface X. |
| `USBD_X_PACKET_SIZE_BYTES` | `<value>` | Maximum packet size of the device interface X. |
