/*
 * usb_class.h
 *
 *  Created on: 02 sep. 2024
 *      Author: Ludo
 */

#ifndef __USB_CLASS_H__
#define __USB_CLASS_H__

#include "types.h"

/*** USB CLASS structures ***/

/*!******************************************************************
 * \enum USB_class_code_t
 * \brief USB class codes list.
 *******************************************************************/
typedef enum {
    USB_CLASS_CODE_USE_INTERFACE = 0x00,
    USB_CLASS_CODE_AUDIO = 0x01,
    USB_CLASS_CODE_CDC_CONTROL = 0x02,
    USB_CLASS_CODE_HID = 0x03,
    USB_CLASS_CODE_PHYSICAL = 0x05,
    USB_CLASS_CODE_IMAGE = 0x06,
    USB_CLASS_CODE_PRINTER = 0x07,
    USB_CLASS_CODE_MASS_STORAGE = 0x08,
    USB_CLASS_CODE_HUB = 0x09,
    USB_CLASS_CODE_CDC_DATA = 0x0A,
    USB_CLASS_CODE_SMART_CARD = 0x0B,
    USB_CLASS_CODE_CONTENT_SECURITY = 0x0D,
    USB_CLASS_CODE_VIDEO = 0x0E,
    USB_CLASS_CODE_PERSONAL_HEALTHCARE = 0x0F,
    USB_CLASS_CODE_AUDIO_VIDEO = 0x10,
    USB_CLASS_CODE_BILLBOARD = 0x11,
    USB_CLASS_CODE_TYPE_C_BRIDGE = 0x12,
    USB_CLASS_CODE_I3C = 0x3C,
    USB_CLASS_CODE_DIAGNOSTIC = 0xDC,
    USB_CLASS_CODE_WIRELESS_CONTROLLER = 0xE0,
    USB_CLASS_CODE_MISCELLANEOUS = 0xEF,
    USB_CLASS_CODE_APPLICATION_SPECIFIC = 0xFE,
    USB_CLASS_CODE_VENDOR_SPECIFIC = 0xFF
} USB_class_code_t;

#endif /* __USB_CLASS_H__ */
