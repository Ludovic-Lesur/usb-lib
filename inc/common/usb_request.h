/*
 * usb_request.h
 *
 *  Created on: 02 sep. 2024
 *      Author: Ludo
 */

#ifndef __USB_REQUEST_H__
#define __USB_REQUEST_H__

#include "common/usb_types.h"
#include "types.h"

/*** USB REQUEST structures ***/

/*!******************************************************************
 * \enum USB_request_direction_t
 * \brief USB request directions list.
 *******************************************************************/
typedef enum {
    USB_REQUEST_DIRECTION_HOST_TO_DEVICE = 0x00,
    USB_REQUEST_DIRECTION_DEVICE_TO_HOST = 0x01,
    USB_REQUEST_DIRECTION_LAST
} USB_request_direction_t;

/*!******************************************************************
 * \enum USB_request_type_t
 * \brief USB request types list.
 *******************************************************************/
typedef enum {
    USB_REQUEST_TYPE_STANDARD = 0x00,
    USB_REQUEST_TYPE_CLASS = 0x01,
    USB_REQUEST_TYPE_VENDOR = 0x02,
    USB_REQUEST_TYPE_LAST
} USB_request_type_t;

/*!******************************************************************
 * \enum USB_request_recipient_t
 * \brief USB request recipients list.
 *******************************************************************/
typedef enum {
    USB_REQUEST_RECIPIENT_DEVICE = 0x00,
    USB_REQUEST_RECIPIENT_INTERFACE = 0x01,
    USB_REQUEST_RECIPIENT_ENDPOINT = 0x02,
    USB_REQUEST_RECIPIENT_OTHER = 0x03,
    USB_REQUEST_RECIPIENT_LAST
} USB_request_recipient_t;

/*!******************************************************************
 * \enum USB_request_standard_t
 * \brief USB standard requests list.
 *******************************************************************/
typedef enum {
    USB_REQUEST_GET_STATUS = 0x00,
    USB_REQUEST_CLEAR_FEATURE = 0x01,
    USB_REQUEST_SET_FEATURE = 0x03,
    USB_REQUEST_SET_ADDRESS = 0x05,
    USB_REQUEST_GET_DESCRIPTOR = 0x06,
    USB_REQUEST_SET_DESCRIPTOR = 0x07,
    USB_REQUEST_GET_CONFIGURATION = 0x08,
    USB_REQUEST_SET_CONFIGURATION = 0x09,
    USB_REQUEST_GET_INTERFACE = 0x0A,
    USB_REQUEST_SET_INTERFACE = 0x0B,
    USB_REQUEST_SYNCH_FRAME = 0x0C,
    USB_REQUEST_LAST
} USB_request_standard_t;

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
 * \struct USB_request_bmRequestType_t
 * \brief USB request type descriptor format.
 *******************************************************************/
typedef union {
    struct {
        uint8_t recipient :5;
        uint8_t type :2;
        uint8_t direction :1;
    } __attribute__((packed));
    uint8_t value;
} USB_request_bmRequestType_t;

/*!******************************************************************
 * \struct USB_request_t
 * \brief USB request structure.
 *******************************************************************/
typedef struct {
    USB_request_bmRequestType_t bmRequestType;
    uint8_t bRequest;
    uint16_t wValue;
    uint16_t wIndex;
    uint16_t wLength;
} __attribute__((packed)) USB_request_t;

/*!******************************************************************
 * \fn USB_request_cb_t
 * \brief USB request execution callback.
 *******************************************************************/
typedef USB_status_t (*USB_request_cb_t)(USB_request_t* request, USB_data_t* data_out, USB_data_t* data_in);

#endif /* __USB_REQUEST_H__ */
