/*
 * usb_cdc.h
 *
 *  Created on: 17 apr. 2025
 *      Author: Ludo
 */

#ifndef __USB_CDC_H__
#define __USB_CDC_H__

/*** USB CDC macros ***/

#define USB_CDC_DESCRIPTOR_VERSION  0x0120

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

/*!******************************************************************
 * \enum USB_CDC_descriptor_subtype_t
 * \brief USB CDC specific descriptor types list.
 *******************************************************************/
typedef enum {
    USB_CDC_DESCRIPTOR_SUBTYPE_HEADER = 0x00,
    USB_CDC_DESCRIPTOR_SUBTYPE_CALL = 0x01,
    USB_CDC_DESCRIPTOR_SUBTYPE_ABSTRACT = 0x02,
    USB_CDC_DESCRIPTOR_SUBTYPE_DIRECT_LINE = 0x03,
    USB_CDC_DESCRIPTOR_SUBTYPE_TELEPHONE_RINGER = 0x04,
    USB_CDC_DESCRIPTOR_SUBTYPE_TELEPHONE_CALL = 0x05,
    USB_CDC_DESCRIPTOR_SUBTYPE_UNION = 0x06,
    USB_CDC_DESCRIPTOR_SUBTYPE_COUNTRY_SELECTION = 0x07,
    USB_CDC_DESCRIPTOR_SUBTYPE_TELEPHONE_OPERATIONAL_MODES = 0x08,
    USB_CDC_DESCRIPTOR_SUBTYPE_USB_TERMINAL = 0x09,
    USB_CDC_DESCRIPTOR_SUBTYPE_NETWORK_CHANNEL_TERMINAL = 0x0A,
    USB_CDC_DESCRIPTOR_SUBTYPE_PROTOCOL_UNIT = 0x0B,
    USB_CDC_DESCRIPTOR_SUBTYPE_EXTENSION_UNIT = 0x0C,
    USB_CDC_DESCRIPTOR_SUBTYPE_MULTI_CHANNEL = 0x0D,
    USB_CDC_DESCRIPTOR_SUBTYPE_CAPI = 0x0E,
    USB_CDC_DESCRIPTOR_SUBTYPE_ETHERNET_NETWORKING = 0x0F,
    USB_CDC_DESCRIPTOR_SUBTYPE_ATM_NETWORKING = 0x10,
    USB_CDC_DESCRIPTOR_SUBTYPE_WIRELESS_HANDSET = 0x11,
    USB_CDC_DESCRIPTOR_SUBTYPE_MOBILE_DIRECT_LINE = 0x12,
    USB_CDC_DESCRIPTOR_SUBTYPE_MDLM_DETAIL = 0x13,
    USB_CDC_DESCRIPTOR_SUBTYPE_DEVICE = 0x14,
    USB_CDC_DESCRIPTOR_SUBTYPE_OBEX = 0x15,
    USB_CDC_DESCRIPTOR_SUBTYPE_COMMAND_SET = 0x16,
    USB_CDC_DESCRIPTOR_SUBTYPE_COMMAND_SET_DETAIL = 0x17,
    USB_CDC_DESCRIPTOR_SUBTYPE_TELEPHONE_CONTROL = 0x18,
    USB_CDC_DESCRIPTOR_SUBTYPE_OBEX_SERVICE_IDENTIFIER = 0x19,
    USB_CDC_DESCRIPTOR_SUBTYPE_NCM = 0x1A
} USB_CDC_descriptor_subtype_t;

/*!******************************************************************
 * \struct USB_CDC_functional_descriptor_t
 * \brief USB CDC specific header descriptor.
 *******************************************************************/
typedef struct {
    uint8_t bFunctionLength;
    USB_descriptor_type_t bDescriptorType;
    USB_CDC_descriptor_subtype_t bDescriptorSubtype;
    uint16_t bcdCDC;
} __attribute__((packed)) USB_CDC_header_descriptor_t;

/*!******************************************************************
 * \struct USB_CDC_call_capabilities_descriptor_t
 * \brief USB CDC call capabilities format.
 *******************************************************************/
typedef union {
    struct {
        uint8_t handle_itself :1;
        uint8_t call_class :1;
        uint8_t reserved_7_2 :6;
    } __attribute__((packed));
    uint8_t value;
} USB_CDC_call_capabilities_descriptor_t;

/*!******************************************************************
 * \struct USB_CDC_call_descriptor_t
 * \brief USB CDC specific call management descriptor.
 *******************************************************************/
typedef struct {
    uint8_t bFunctionLength;
    USB_descriptor_type_t bDescriptorType;
    USB_CDC_descriptor_subtype_t bDescriptorSubtype;
    USB_CDC_call_capabilities_descriptor_t bmCapabilities;
    uint8_t bDataInterface;
} __attribute__((packed)) USB_CDC_call_descriptor_t;

/*!******************************************************************
 * \struct USB_CDC_abstract_capabilities_descriptor_t
 * \brief USB CDC abstract capabilities format.
 *******************************************************************/
typedef union {
    struct {
        uint8_t feature :1;
        uint8_t line_coding :1;
        uint8_t send_break :1;
        uint8_t network_connection :1;
        uint8_t reserved_7_4 :4;
    } __attribute__((packed));
    uint8_t value;
} USB_CDC_abstract_capabilities_descriptor_t;

/*!******************************************************************
 * \struct USB_CDC_abstract_descriptor_t
 * \brief USB CDC specific abstract management descriptor.
 *******************************************************************/
typedef struct {
    uint8_t bFunctionLength;
    USB_descriptor_type_t bDescriptorType;
    USB_CDC_descriptor_subtype_t bDescriptorSubtype;
    USB_CDC_abstract_capabilities_descriptor_t bmCapabilities;
} __attribute__((packed)) USB_CDC_abstract_descriptor_t;

/*!******************************************************************
 * \struct USB_CDC_union_descriptor_t
 * \brief USB CDC specific union descriptor.
 *******************************************************************/
typedef struct {
    uint8_t bFunctionLength;
    USB_descriptor_type_t bDescriptorType;
    USB_CDC_descriptor_subtype_t bDescriptorSubtype;
    uint8_t bControlInterface;
    uint8_t bSubordinateInterface;
} __attribute__((packed)) USB_CDC_union_descriptor_t;

#endif /* __USB_CDC_H__ */
