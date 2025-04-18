/*
 * usb_device.h
 *
 *  Created on: 02 sep. 2024
 *      Author: Ludo
 */

#ifndef __USB_DEVICE_H__
#define __USB_DEVICE_H__

#include "common/usb_class.h"
#include "common/usb_configuration.h"
#include "common/usb_descriptor.h"
#include "common/usb_types.h"
#include "types.h"

/*** USB DEVICE macros ***/

#define USB_DEVICE_DESCRIPTOR_USB_VERSION   0x0200

/*** USB DEVICE structures ***/

/*!******************************************************************
 * \struct USB_device_descriptor_t
 * \brief USB device descriptor structure.
 *******************************************************************/
typedef struct {
    uint8_t bLength;
    USB_descriptor_type_t bDescriptorType;
    uint16_t bcdUSB;
    USB_class_code_t bDeviceClass;
    uint8_t bDeviceSubClass;
    uint8_t bDeviceProtocol;
    uint8_t bMaxPacketSize0;
    uint16_t idVendor;
    uint16_t idProduct;
    uint16_t bcdDevice;
    uint8_t iManufacturer;
    uint8_t iProduct;
    uint8_t iSerialNumber;
    uint8_t bNumConfigurations;
} __attribute__((packed)) USB_device_descriptor_t;

/*!******************************************************************
 * \struct USB_device_descriptor_t
 * \brief USB device descriptor structure.
 *******************************************************************/
typedef struct {
    uint8_t bLength;
    USB_descriptor_type_t bDescriptorType;
    uint16_t bcdUSB;
    USB_class_code_t bDeviceClass;
    uint8_t bDeviceSubClass;
    uint8_t bDeviceProtocol;
    uint8_t bMaxPacketSize0;
    uint8_t bNumConfigurations;
    uint8_t bReserved;
} __attribute__((packed)) USB_device_qualifier_descriptor_t;

/*!******************************************************************
 * \struct USB_device_t
 * \brief USB device structure.
 *******************************************************************/
typedef struct {
    const USB_device_descriptor_t* descriptor;
    const USB_device_qualifier_descriptor_t* qualifier_descriptor;
    const USB_configuration_t** configuration_list;
    const uint8_t number_of_configurations;
    const char_t** string_descriptor_list;
    const uint8_t number_of_string_descriptors;
} USB_device_t;

#endif /* __USB_DEVICE_H__ */
