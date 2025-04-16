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

/*!******************************************************************
 * \enum USB_cdc_subclass_code_t
 * \brief USB CDC sub-class codes list.
 *******************************************************************/
typedef enum {
    USB_CDC_SUBCLASS_CODE_DIRECT_LINE = 0x01,
    USB_CDC_SUBCLASS_CODE_ABSTRACT = 0x02,
    USB_CDC_SUBCLASS_CODE_TELEPHONE = 0x03,
    USB_CDC_SUBCLASS_CODE_MULTI_CHANNEL = 0x04,
    USB_CDC_SUBCLASS_CODE_CAPI = 0x05,
    USB_CDC_SUBCLASS_CODE_ETHERNET_NETWORKING = 0x06,
    USB_CDC_SUBCLASS_CODE_ATM_NETWORKING = 0x07,
    USB_CDC_SUBCLASS_CODE_WIRELESS_HANDSET = 0x08,
    USB_CDC_SUBCLASS_CODE_DEVICE_MANAGEMENT = 0x09,
    USB_CDC_SUBCLASS_CODE_MOBILE_DIRECT = 0x0A,
    USB_CDC_SUBCLASS_CODE_OBEX = 0x0B,
    USB_CDC_SUBCLASS_CODE_ETHERNET_EMULATION = 0x0C,
    USB_CDC_SUBCLASS_CODE_NETWORK_CONTROL = 0x0D
} USB_cdc_subclass_code_t;

/*!******************************************************************
 * \enum USB_cdc_class_protocol_code_t
 * \brief USB CDC class protocol codes list.
 *******************************************************************/
typedef enum {
    USB_CDC_PROTOCOL_CODE_NONE = 0x00,
    USB_CDC_PROTOCOL_CODE_AT_COMMANDS_V_250 = 0x01,
    USB_CDC_PROTOCOL_CODE_AT_COMMANDS_PCCA_101 = 0x02,
    USB_CDC_PROTOCOL_CODE_AT_COMMANDS_PCCA_101_ANNEX_O = 0x03,
    USB_CDC_PROTOCOL_CODE_AT_COMMANDS_GSM_07_07 = 0x04,
    USB_CDC_PROTOCOL_CODE_AT_COMMANDS_3GPP_27_007 = 0x05,
    USB_CDC_PROTOCOL_CODE_AT_COMMANDS_TIA_CDMA = 0x06,
    USB_CDC_PROTOCOL_CODE_EEM = 0x07,
    USB_CDC_PROTOCOL_CODE_EXTERNAL = 0xFE,
    USB_CDC_PROTOCOL_CODE_VENDOR_SPECIFIC = 0xFF
} USB_cdc_protocol_code_t;

#endif /* __USB_CLASS_H__ */
