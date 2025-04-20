/*
 * usb_interface.h
 *
 *  Created on: 02 sep. 2024
 *      Author: Ludo
 */

#ifndef __USB_INTERFACE_H__
#define __USB_INTERFACE_H__

#include "common/usb_class.h"
#include "common/usb_descriptor.h"
#include "common/usb_endpoint.h"
#include "common/usb_request.h"
#include "types.h"

/*** USB INTERFACE structures ***/

/*!******************************************************************
 * \struct USB_interface_descriptor_t
 * \brief USB interface descriptor structure.
 *******************************************************************/
typedef struct {
    uint8_t bLength;
    USB_descriptor_type_t bDescriptorType;
    uint8_t bInterfaceNumber;
    uint8_t bAlternateSetting;
    uint8_t bNumEndpoints;
    USB_class_code_t bInterfaceClass;
    uint8_t bInterfaceSubClass;
    uint8_t bInterfaceProtocol;
    uint8_t iInterface;
} __attribute__((packed)) USB_interface_descriptor_t;

/*!******************************************************************
 * \struct USB_interface_t
 * \brief USB interface structure.
 *******************************************************************/
typedef struct {
    const USB_interface_descriptor_t* descriptor;
    const USB_endpoint_t** endpoint_list;
    const uint8_t number_of_endpoints;
    const uint8_t** cs_descriptor;
    const uint8_t* cs_descriptor_length;
    USB_request_cb_t request_callback;
} USB_interface_t;

#endif /* __USB_INTERFACE_H__ */
