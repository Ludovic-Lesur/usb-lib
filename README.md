# Description

This repository contains a cross-platform USB stack for embedded projects.

| USB features | Device |
|:---:|:---:|
| **Control pipe** | :hammer: |
| **CDC-ACM** class | :hammer: |

# Dependencies

The driver relies on:

* An external `types.h` header file defining the **standard C types** of the targeted MCU.
* The **embedded utility functions** defined in the [embedded-utils](https://github.com/Ludovic-Lesur/embedded-utils) repository.

Here is the versions compatibility table:

| **usb-lib** | **embedded-utils** |
|:---:|:---:|
| [sw1.0](https://github.com/Ludovic-Lesur/usb-lib/releases/tag/sw1.0) | >= [sw7.0](https://github.com/Ludovic-Lesur/embedded-utils/releases/tag/sw7.0) |

# Compilation flags

| **Flag name** | **Value** | **Description** |
|:---:|:---:|:---:|
| `USB_LIB_DISABLE_FLAGS_FILE` | `defined` / `undefined` | Disable the `usb_lib_flags.h` header file inclusion when compilation flags are given in the project settings or by command line. |
| `USB_LIB_DISABLE` | `defined` / `undefined` | Disable the USB library. |
| `USB_LIB_HW_INTERFACE_ERROR_BASE_LAST` | `defined` / `undefined` | Last error base of the low level USB driver. |
| `USBD_CONTROL_INTERFACE_INDEX` | `<value>` | Index of the device control interface. |
| `USBD_CONTROL_INTERFACE_STRING_DESCRIPTOR_INDEX` | `<value>` | Index of the device control interface string descriptor. |
| `USBD_CDC` | `defined` / `undefined` | Enable the CDC device class if defined. |
| `USBD_CDC_COM_INTERFACE_INDEX` | `<value>` | Index of the device CDC COM interface. |
| `USBD_CDC_COM_INTERFACE_STRING_DESCRIPTOR_INDEX` | `<value>` | Index of the device CDC COM interface string descriptor. |
| `USBD_CDC_COM_ENDPOINT_NUMBER` | `<value>` | Endpoint number assigned to the device CDC COM interface. |
| `USBD_CDC_COM_PACKET_SIZE_BYTES` | `<value>` | Maximum packet size of the device CDC COM interface. |
| `USBD_CDC_DATA_INTERFACE_INDEX` | `<value>` | Index of the device CDC DATA interface. |
| `USBD_CDC_DATA_INTERFACE_STRING_DESCRIPTOR_INDEX` | `<value>` | Index of the device CDC DATA interface string descriptor. |
| `USBD_CDC_DATA_ENDPOINT_NUMBER` | `<value>` | Endpoint number assigned to the device CDC DATA interface. |
| `USBD_CDC_DATA_PACKET_SIZE_BYTES` | `<value>` | Maximum packet size of the device CDC DATA interface. |
