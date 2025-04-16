/*
 * usb_descriptor.h
 *
 *  Created on: 03 nov. 2024
 *      Author: Ludo
 */

#ifndef __USB_DESCRIPTOR_H__
#define __USB_DESCRIPTOR_H__

/*!******************************************************************
 * \enum USB_descriptor_type_t
 * \brief USB descriptor types list.
 *******************************************************************/
typedef enum {
    USB_DESCRIPTOR_TYPE_DEVICE = 1,
    USB_DESCRIPTOR_TYPE_CONFIGURATION = 2,
    USB_DESCRIPTOR_TYPE_STRING = 3,
    USB_DESCRIPTOR_TYPE_INTERFACE = 4,
    USB_DESCRIPTOR_TYPE_ENDPOINT = 5,
    USB_DESCRIPTOR_TYPE_DEVICE_QUALIFIER = 6,
    USB_DESCRIPTOR_TYPE_OTHER_SPEED_CONFIGURATION = 7,
    USB_DESCRIPTOR_TYPE_INTERFACE_POWER = 8,
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
