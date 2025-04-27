/*
 * usb_descriptor.h
 *
 *  Created on: 03 nov. 2024
 *      Author: Ludo
 */

#ifndef __USB_DESCRIPTOR_H__
#define __USB_DESCRIPTOR_H__

#include "common/usb_descriptor.h"
#include "types.h"

/*** USB DESCRIPTOR macros ***/

#define USB_STRING_DESCRIPTOR_INDEX_LANGID  0

/*** USB DESCRIPTOR structures ***/

/*!******************************************************************
 * \enum USB_descriptor_type_t
 * \brief USB descriptor types list.
 *******************************************************************/
typedef enum {
    USB_DESCRIPTOR_TYPE_DEVICE = 0x01,
    USB_DESCRIPTOR_TYPE_CONFIGURATION = 0x02,
    USB_DESCRIPTOR_TYPE_STRING = 0x03,
    USB_DESCRIPTOR_TYPE_INTERFACE = 0x04,
    USB_DESCRIPTOR_TYPE_ENDPOINT = 0x05,
    USB_DESCRIPTOR_TYPE_DEVICE_QUALIFIER = 0x06,
    USB_DESCRIPTOR_TYPE_OTHER_SPEED_CONFIGURATION = 0x07,
    USB_DESCRIPTOR_TYPE_INTERFACE_POWER = 0x08,
    USB_DESCRIPTOR_TYPE_OTG = 0x09,
    USB_DESCRIPTOR_TYPE_DEBUG = 0x0A,
    USB_DESCRIPTOR_TYPE_INTERFACE_ASSOCIATION = 0x0B,
    USB_DESCRIPTOR_TYPE_CLASS_SPECIFIC_UNDEFINED = 0x20,
    USB_DESCRIPTOR_TYPE_CLASS_SPECIFIC_DEVICE = 0x21,
    USB_DESCRIPTOR_TYPE_CLASS_SPECIFIC_CONFIGURATION = 0x22,
    USB_DESCRIPTOR_TYPE_CLASS_SPECIFIC_STRING = 0x23,
    USB_DESCRIPTOR_TYPE_CLASS_SPECIFIC_INTERFACE = 0x24,
    USB_DESCRIPTOR_TYPE_CLASS_SPECIFIC_ENDPOINT = 0x25,
    USB_DESCRIPTOR_TYPE_LAST
} USB_descriptor_type_t;

/*!******************************************************************
 * \struct USB_string_descriptor_t
 * \brief USB string descriptor structure.
 *******************************************************************/
typedef struct {
    uint8_t bLength;
    USB_descriptor_type_t bDescriptorType;
} __attribute__((packed)) USB_string_descriptor_t;

#endif /* __USB_DESCRIPTOR_H__ */
