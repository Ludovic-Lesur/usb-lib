/*
 * usb_types.h
 *
 *  Created on: 30 apr. 2023
 *      Author: Ludo
 */

#ifndef __USB_TYPES_H__
#define __USB_TYPES_H__

#ifndef USB_LIB_DISABLE_FLAGS_FILE
#include "usb_lib_flags.h"
#endif
#include "error.h"
#include "string.h"
#include "types.h"

/*** USB TYPES macros ***/

#define USB_SETUP_PACKET_SIZE_BYTES     8

#define USB_LS_CONTROL_PACKET_SIZE_MAX  8
#define USB_FS_CONTROL_PACKET_SIZE_MAX  64
#define USB_HS_CONTROL_PACKET_SIZE_MAX  64

/*** USB TYPES structures ***/

/*!******************************************************************
 * \enum USBD_status_t
 * \brief USB device drivers error codes.
 *******************************************************************/
typedef enum {
    // Common errors.
    USB_SUCCESS = 0,
    USB_ERROR_NULL_PARAMETER,
    USB_ERROR_ALREADY_INITIALIZED,
    USB_ERROR_UNINITIALIZED,
    USB_ERROR_CONFIGURATION_DESCRIPTOR_SIZE,
    USB_ERROR_CONFIGURATION_VALUE,
    USB_ERROR_ENDPOINT_NUMBER,
    USB_ERROR_ENDPOINT_DIRECTION,
    USB_ERROR_ENDPOINT_BUFFER_MODE,
    USB_ERROR_REQUEST_TYPE,
    USB_ERROR_REQUEST_SIZE,
    USB_ERROR_STANDARD_REQUEST,
    USB_ERROR_CLASS_REQUEST,
    USB_ERROR_VENDOR_REQUEST,
    USB_ERROR_DESCRIPTOR_TYPE,
    USB_ERROR_CONFIGURATION_INDEX,
    USB_ERROR_STRING_DESCRIPTOR_INDEX,
    USB_ERROR_CS_DESCRIPTOR_SIZE,
    // CDC errors.
    USB_ERROR_CDC_FEATURE,
    USB_ERROR_CDC_DATA_SIZE,
    // Low level drivers errors.
    USB_ERROR_BASE_HW_INTERFACE = ERROR_BASE_STEP,
    USB_ERROR_BASE_STRING = (USB_ERROR_BASE_HW_INTERFACE + USB_LIB_HW_INTERFACE_ERROR_BASE_LAST),
    // Last base value.
    USB_ERROR_BASE_LAST = (USB_ERROR_BASE_STRING + STRING_ERROR_BASE_LAST)
} USB_status_t;

/*!******************************************************************
 * \struct USB_data_t
 * \brief USB data structure.
 *******************************************************************/
typedef struct {
    uint8_t* data;
    uint32_t size_bytes;
} USB_data_t;

/*******************************************************************/
#define USB_exit_error(base) { ERROR_check_exit(usb_status, USB_SUCCESS, base) }

/*******************************************************************/
#define USB_stack_error(base) { ERROR_check_stack(usb_status, USB_SUCCESS, base) }

/*******************************************************************/
#define USB_stack_exit_error(base, code) { ERROR_check_stack_exit(usb_status, USB_SUCCESS, base, code) }

#endif /* __USB_TYPES_H__ */
