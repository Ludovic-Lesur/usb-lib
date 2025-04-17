/*
 * usb_endpoint.h
 *
 *  Created on: 02 sep. 2024
 *      Author: Ludo
 */

#ifndef __USB_ENDPOINT_H__
#define __USB_ENDPOINT_H__

#include "common/usb_descriptor.h"
#include "common/usb_request.h"
#include "types.h"

/*** USB ENDPOINT structures ***/

/*!******************************************************************
 * \fn USB_endpoint_cb_t
 * \brief USB endpoint transfer completion callback.
 *******************************************************************/
typedef void (*USB_endpoint_cb_t)(void);

/*!******************************************************************
 * \fn USB_setup_cb_t
 * \brief USB setup packet reception callback.
 *******************************************************************/
typedef void (*USB_setup_cb_t)(USB_request_operation_t* request_operation);

/*!******************************************************************
 * \enum USB_endpoint_direction_t
 * \brief USB endpoint directions list.
 *******************************************************************/
typedef enum {
    USB_ENDPOINT_DIRECTION_OUT = 0,
    USB_ENDPOINT_DIRECTION_IN,
    USB_ENDPOINT_DIRECTION_LAST
} USB_endpoint_direction_t;

/*!******************************************************************
 * \enum USB_endpoint_transfer_type_t
 * \brief USB endpoint transfer types list.
 *******************************************************************/
typedef enum {
    USB_ENDPOINT_TRANSFER_TYPE_CONTROL = 0,
    USB_ENDPOINT_TRANSFER_TYPE_ISOCHRONOUS,
    USB_ENDPOINT_TRANSFER_TYPE_BULK,
    USB_ENDPOINT_TRANSFER_TYPE_INTERRUPT,
    USB_ENDPOINT_TRANSFER_TYPE_LAST
} USB_endpoint_transfer_type_t;

/*!******************************************************************
 * \enum USB_endpoint_synchronization_type_t
 * \brief USB endpoint synchronization types list.
 *******************************************************************/
typedef enum {
    USB_ENDPOINT_SYNCHRONIZATION_TYPE_NONE = 0,
    USB_ENDPOINT_SYNCHRONIZATION_TYPE_ASYNCHRONOUS,
    USB_ENDPOINT_SYNCHRONIZATION_TYPE_ADAPTATIVE,
    USB_ENDPOINT_SYNCHRONIZATION_TYPE_SYNCHRONOUS,
    USB_ENDPOINT_SYNCHRONIZATION_TYPE_LAST
} USB_endpoint_synchronization_type_t;

/*!******************************************************************
 * \enum USB_endpoint_usage_type_t
 * \brief USB endpoint usage types list.
 *******************************************************************/
typedef enum {
    USB_ENDPOINT_USAGE_TYPE_DATA = 0,
    USB_ENDPOINT_USAGE_TYPE_FEEDBACK,
    USB_ENDPOINT_USAGE_TYPE_IMPLICIT_FEEDBACK_DATA,
    USB_ENDPOINT_USAGE_TYPE_LAST
} USB_endpoint_usage_type_t;

/*!******************************************************************
 * \struct USB_endpoint_address_descriptor_t
 * \brief USB endpoint address descriptor format.
 *******************************************************************/
typedef union {
    struct {
        uint8_t number :4;
        uint8_t reserved_6_4 :3;
        uint8_t direction :1;
    } __attribute__((packed));
    uint8_t value;
} USB_endpoint_address_descriptor_t;

/*!******************************************************************
 * \struct USB_endpoint_attributes_descriptor_t
 * \brief USB endpoint attributes descriptor format.
 *******************************************************************/
typedef union {
    struct {
        uint8_t transfer_type :2;
        uint8_t synchronization_type :2;
        uint8_t usage_type :2;
        uint8_t reserved_7_6 :2;
    } __attribute__((packed));
    uint8_t value;
} USB_endpoint_attributes_descriptor_t;

/*!******************************************************************
 * \struct USB_endpoint_max_packet_size_descriptor_t
 * \brief USB endpoint attributes descriptor format.
 *******************************************************************/
typedef union {
    struct {
        uint16_t max_packet_size :11;
        uint8_t transaction_per_microframe :2;
        uint8_t reserved_15_13 :3;
    } __attribute__((packed));
    uint16_t value;
} USB_endpoint_max_packet_size_descriptor_t;

/*!******************************************************************
 * \struct USB_endpoint_descriptor_t
 * \brief USB endpoint descriptor structure.
 *******************************************************************/
typedef struct {
    uint8_t bLength;
    USB_descriptor_type_t bDescriptorType;
    USB_endpoint_address_descriptor_t bEndpointAddress;
    USB_endpoint_attributes_descriptor_t bmAttributes;
    USB_endpoint_max_packet_size_descriptor_t wMaxPacketSize;
    uint8_t bInterval;
} __attribute__((packed)) USB_endpoint_descriptor_t;

/*!******************************************************************
 * \struct USB_physical_endpoint_t
 * \brief USB physical endpoint descriptor structure.
 *******************************************************************/
typedef struct {
    uint8_t number;
    USB_endpoint_direction_t direction;
    USB_endpoint_transfer_type_t transfer_type;
    USB_endpoint_synchronization_type_t synchronization_type;
    USB_endpoint_usage_type_t usage_type;
    uint16_t max_packet_size_bytes;
    USB_endpoint_cb_t callback;
} USB_physical_endpoint_t;

/*!******************************************************************
 * \struct USB_endpoint_t
 * \brief USB endpoint structure.
 *******************************************************************/
typedef struct {
    const USB_endpoint_descriptor_t* descriptor;
    const USB_physical_endpoint_t* physical_endpoint;
} USB_endpoint_t;

#endif /* __USB_ENDPOINT_H__ */
