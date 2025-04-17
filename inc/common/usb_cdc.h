/*
 * usb_cdc.h
 *
 *  Created on: 17 apr. 2025
 *      Author: Ludo
 */

#ifndef __USB_CDC_H__
#define __USB_CDC_H__

/*** USB CDC structures ***/

/*!******************************************************************
 * \enum USB_CDC_subclass_code_t
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
} USB_CDC_subclass_code_t;

/*!******************************************************************
 * \enum USB_CDC_class_protocol_code_t
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
} USB_CDC_protocol_code_t;

#endif /* __USB_CDC_H__ */
