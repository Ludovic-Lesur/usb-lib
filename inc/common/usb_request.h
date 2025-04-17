/*
 * usb_request.h
 *
 *  Created on: 02 sep. 2024
 *      Author: Ludo
 */

#ifndef __USB_REQUEST_H__
#define __USB_REQUEST_H__

#include "types.h"

/*** USB REQUEST structures ***/

/*!******************************************************************
 * \enum USB_request_direction_t
 * \brief USB request directions list.
 *******************************************************************/
typedef enum {
    USB_REQUEST_DIRECTION_HOST_TO_DEVICE = 0,
    USB_REQUEST_DIRECTION_DEVICE_TO_HOST,
    USB_REQUEST_DIRECTION_LAST
} USB_request_direction_t;

/*!******************************************************************
 * \enum USB_request_sub_type_t
 * \brief USB request sub-types list.
 *******************************************************************/
typedef enum {
    USB_REQUEST_SUB_TYPE_STANDARD = 0,
    USB_REQUEST_SUB_TYPE_CLASS,
    USB_REQUEST_SUB_TYPE_VENDOR,
    USB_REQUEST_SUB_TYPE_LAST
} USB_request_sub_type_t;

/*!******************************************************************
 * \enum USB_request_recipient_t
 * \brief USB request recipients list.
 *******************************************************************/
typedef enum {
    USB_REQUEST_RECIPIENT_DEVICE = 0,
    USB_REQUEST_RECIPIENT_INTERFACE,
    USB_REQUEST_RECIPIENT_ENDPOINT,
    USB_REQUEST_RECIPIENT_OTHER,
    USB_REQUEST_RECIPIENT_LAST
} USB_request_recipient_t;

/*!******************************************************************
 * \enum USB_request_id_t
 * \brief USB request identifiers list.
 *******************************************************************/
typedef enum {
    USB_REQUEST_ID_GET_STATUS = 0,
    USB_REQUEST_ID_CLEAR_FEATURE = 1,
    USB_REQUEST_ID_SET_FEATURE = 3,
    USB_REQUEST_ID_SET_ADDRESS = 5,
    USB_REQUEST_ID_GET_DESCRIPTOR = 6,
    USB_REQUEST_ID_SET_DESCRIPTOR = 7,
    USB_REQUEST_ID_GET_CONFIGURATION = 8,
    USB_REQUEST_ID_SET_CONFIGURATION = 9,
    USB_REQUEST_ID_GET_INTERFACE = 10,
    USB_REQUEST_ID_SET_INTERFACE = 11,
    USB_REQUEST_ID_SYNCH_FRAME = 12,
    USB_REQUEST_ID_LAST
} USB_request_id_t;

/*!******************************************************************
 * \enum USB_request_operation_t
 * \brief USB request operations list.
 *******************************************************************/
typedef enum {
    USB_REQUEST_OPERATION_NOT_SUPPORTED = 0,
    USB_REQUEST_OPERATION_READ,
    USB_REQUEST_OPERATION_WRITE,
    USB_REQUEST_OPERATION_WRITE_NO_DATA,
    USB_REQUEST_OPERATION_LAST
} USB_request_operation_t;

/*!******************************************************************
 * \struct USB_request_type_t
 * \brief USB request type format.
 *******************************************************************/
typedef union {
    struct {
        uint8_t recipient :5;
        uint8_t type :2;
        uint8_t direction :1;
    } __attribute__((packed));
    uint8_t value;
} USB_request_type_t;

/*!******************************************************************
 * \struct USB_request_t
 * \brief USB request structure.
 *******************************************************************/
typedef struct {
    USB_request_type_t bmRequestType;
    USB_request_id_t bRequest;
    uint16_t wValue;
    uint16_t wIndex;
    uint16_t wLength;
} __attribute__((packed)) USB_request_t;

#endif /* __USB_REQUEST_H__ */
