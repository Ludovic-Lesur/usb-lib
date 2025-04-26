/*
 * usbd_uac.c
 *
 *  Created on: 26 apr. 2025
 *      Author: Ludo
 */

#include "device/class/usbd_uac.h"

#ifndef USB_LIB_DISABLE_FLAGS_FILE
#include "usb_lib_flags.h"
#endif
#include "common/usb_device.h"
#include "common/usb_endpoint.h"
#include "common/usb_interface.h"
#include "common/usb_request.h"
#include "common/usb_types.h"
#include "common/usb_uac.h"
#include "device/usbd_hw.h"
#include "types.h"

#if (!(defined USB_LIB_DISABLE) && (defined USBD_UAC))

/*** USBD UAC local macros ***/

#define USBD_UAC_CS_DESCRIPTOR_BUFFER_SIZE_BYTES    1024
#define USBD_UAC_CS_DESCRIPTOR_LENGTH_INDEX         0

/*** USBD UAC local structures ***/

/*******************************************************************/
typedef enum {
    USBD_UAC_CONTROL_ENDPOINT_INDEX_IN = 0,
    USBD_UAC_CONTROL_ENDPOINT_INDEX_LAST
} USBD_UAC_control_endpoint_index_t;

/*******************************************************************/
typedef enum {
    USBD_UAC_STREAM_PLAY_ENDPOINT_INDEX_OUT = 0,
    USBD_UAC_STREAM_PLAY_ENDPOINT_INDEX_LAST
} USBD_UAC_stream_play_endpoint_index_t;

/*******************************************************************/
typedef enum {
    USBD_UAC_STREAM_RECORD_ENDPOINT_INDEX_IN = 0,
    USBD_UAC_STREAM_RECORD_ENDPOINT_INDEX_LAST
} USBD_UAC_stream_record_endpoint_index_t;

/*******************************************************************/
typedef struct {
    USBD_UAC_callbacks_t* callbacks;
    uint8_t cs_descriptor[USBD_UAC_CS_DESCRIPTOR_BUFFER_SIZE_BYTES];
    uint8_t cs_descriptor_length;
    USB_data_t data_out;
    USB_data_t data_in;
} USBD_UAC_context_t;

/*** USBD UAC local functions declaration ***/

static void _USBD_UAC_CONTROL_endpoint_in_callback(void);
static void _USBD_UAC_STREAM_PLAY_endpoint_out_callback(void);
static void _USBD_UAC_STREAM_RECORD_endpoint_in_callback(void);

static USB_status_t _USBD_UAC_CONTROL_request_callback(USB_request_t* request, USB_data_t* data_out, USB_data_t* data_in);

/*** USBD UAC local global variables ***/

static const USB_physical_endpoint_t USBD_UAC_CONTROL_EP_PHY_IN = {
    .number = USBD_UAC_CONTROL_ENDPOINT_NUMBER,
    .direction = USB_ENDPOINT_DIRECTION_IN,
    .transfer_type = USB_ENDPOINT_TRANSFER_TYPE_INTERRUPT,
    .synchronization_type = USB_ENDPOINT_SYNCHRONIZATION_TYPE_NONE,
    .usage_type = USB_ENDPOINT_USAGE_TYPE_DATA,
    .max_packet_size_bytes = USBD_UAC_CONTROL_PACKET_SIZE_BYTES,
    .callback = &_USBD_UAC_CONTROL_endpoint_in_callback
};

static const USB_physical_endpoint_t USBD_UAC_STREAM_PLAY_EP_PHY_OUT = {
    .number = USBD_UAC_STREAM_PLAY_ENDPOINT_NUMBER,
    .direction = USB_ENDPOINT_DIRECTION_OUT,
    .transfer_type = USB_ENDPOINT_TRANSFER_TYPE_ISOCHRONOUS,
    .synchronization_type = USB_ENDPOINT_SYNCHRONIZATION_TYPE_NONE,
    .usage_type = USB_ENDPOINT_USAGE_TYPE_DATA,
    .max_packet_size_bytes = USBD_UAC_STREAM_PLAY_PACKET_SIZE_BYTES,
    .callback = &_USBD_UAC_STREAM_PLAY_endpoint_out_callback
};

static const USB_physical_endpoint_t USBD_UAC_STREAM_RECORD_EP_PHY_IN = {
    .number = USBD_UAC_STREAM_RECORD_ENDPOINT_NUMBER,
    .direction = USB_ENDPOINT_DIRECTION_IN,
    .transfer_type = USB_ENDPOINT_TRANSFER_TYPE_ISOCHRONOUS,
    .synchronization_type = USB_ENDPOINT_SYNCHRONIZATION_TYPE_NONE,
    .usage_type = USB_ENDPOINT_USAGE_TYPE_DATA,
    .max_packet_size_bytes = USBD_UAC_STREAM_RECORD_PACKET_SIZE_BYTES,
    .callback = &_USBD_UAC_STREAM_RECORD_endpoint_in_callback
};

static const USB_endpoint_descriptor_t USBD_UAC_CONTROL_EP_PHY_IN_DESCRIPTOR = {
    .bLength = sizeof(USB_endpoint_descriptor_t),
    .bDescriptorType = USB_DESCRIPTOR_TYPE_ENDPOINT,
    .bEndpointAddress.number = USBD_UAC_CONTROL_EP_PHY_IN.number,
    .bEndpointAddress.direction = USBD_UAC_CONTROL_EP_PHY_IN.direction,
    .bEndpointAddress.reserved_6_4 = 0,
    .bmAttributes.transfer_type = USBD_UAC_CONTROL_EP_PHY_IN.transfer_type,
    .bmAttributes.synchronization_type = USBD_UAC_CONTROL_EP_PHY_IN.synchronization_type,
    .bmAttributes.usage_type = USBD_UAC_CONTROL_EP_PHY_IN.usage_type,
    .bmAttributes.reserved_7_6 = 0,
    .wMaxPacketSize.max_packet_size = USBD_UAC_CONTROL_EP_PHY_IN.max_packet_size_bytes,
    .wMaxPacketSize.transaction_per_microframe = 0,
    .wMaxPacketSize.reserved_15_13 = 0,
    .bInterval = 255
};

static const USB_endpoint_descriptor_t USBD_UAC_STREAM_EP_PHY_OUT_DESCRIPTOR = {
    .bLength = sizeof(USB_endpoint_descriptor_t),
    .bDescriptorType = USB_DESCRIPTOR_TYPE_ENDPOINT,
    .bEndpointAddress.number = USBD_UAC_STREAM_PLAY_EP_PHY_OUT.number,
    .bEndpointAddress.direction = USBD_UAC_STREAM_PLAY_EP_PHY_OUT.direction,
    .bEndpointAddress.reserved_6_4 = 0,
    .bmAttributes.transfer_type = USBD_UAC_STREAM_PLAY_EP_PHY_OUT.transfer_type,
    .bmAttributes.synchronization_type = USBD_UAC_STREAM_PLAY_EP_PHY_OUT.synchronization_type,
    .bmAttributes.usage_type = USBD_UAC_STREAM_PLAY_EP_PHY_OUT.usage_type,
    .bmAttributes.reserved_7_6 = 0,
    .wMaxPacketSize.max_packet_size = USBD_UAC_STREAM_PLAY_EP_PHY_OUT.max_packet_size_bytes,
    .wMaxPacketSize.transaction_per_microframe = 0,
    .wMaxPacketSize.reserved_15_13 = 0,
    .bInterval = 1
};

static const USB_endpoint_descriptor_t USBD_UAC_STREAM_EP_PHY_IN_DESCRIPTOR = {
    .bLength = sizeof(USB_endpoint_descriptor_t),
    .bDescriptorType = USB_DESCRIPTOR_TYPE_ENDPOINT,
    .bEndpointAddress.number = USBD_UAC_STREAM_RECORD_EP_PHY_IN.number,
    .bEndpointAddress.direction = USBD_UAC_STREAM_RECORD_EP_PHY_IN.direction,
    .bEndpointAddress.reserved_6_4 = 0,
    .bmAttributes.transfer_type = USBD_UAC_STREAM_RECORD_EP_PHY_IN.transfer_type,
    .bmAttributes.synchronization_type = USBD_UAC_STREAM_RECORD_EP_PHY_IN.synchronization_type,
    .bmAttributes.usage_type = USBD_UAC_STREAM_RECORD_EP_PHY_IN.usage_type,
    .bmAttributes.reserved_7_6 = 0,
    .wMaxPacketSize.max_packet_size = USBD_UAC_STREAM_RECORD_EP_PHY_IN.max_packet_size_bytes,
    .wMaxPacketSize.transaction_per_microframe = 0,
    .wMaxPacketSize.reserved_15_13 = 0,
    .bInterval = 1
};

static const USB_endpoint_t USBD_UAC_CONTROL_EP_IN = {
    .physical_endpoint = &USBD_UAC_CONTROL_EP_PHY_IN,
    .descriptor = &USBD_UAC_CONTROL_EP_PHY_IN_DESCRIPTOR
};

static const USB_endpoint_t USBD_UAC_STREAM_PLAY_EP_OUT = {
    .physical_endpoint = &USBD_UAC_STREAM_PLAY_EP_PHY_OUT,
    .descriptor = &USBD_UAC_STREAM_EP_PHY_OUT_DESCRIPTOR
};

static const USB_endpoint_t USBD_UAC_STREAM_RECORD_EP_IN = {
    .physical_endpoint = &USBD_UAC_STREAM_RECORD_EP_PHY_IN,
    .descriptor = &USBD_UAC_STREAM_EP_PHY_IN_DESCRIPTOR
};

static const USB_endpoint_t* const USBD_UAC_CONTROL_INTERFACE_EP_LIST[USBD_UAC_CONTROL_ENDPOINT_INDEX_LAST] = {
    &USBD_UAC_CONTROL_EP_IN
};

static const USB_endpoint_t* const USBD_UAC_STREAM_PLAY_INTERFACE_EP_LIST[USBD_UAC_STREAM_PLAY_ENDPOINT_INDEX_LAST] = {
    &USBD_UAC_STREAM_PLAY_EP_OUT,
};

static const USB_endpoint_t* const USBD_UAC_STREAM_RECORD_INTERFACE_EP_LIST[USBD_UAC_STREAM_RECORD_ENDPOINT_INDEX_LAST] = {
    &USBD_UAC_STREAM_RECORD_EP_IN,
};

static const USB_interface_descriptor_t USB_UAC_CONTROL_INTERFACE_DESCRIPTOR = {
    .bLength = sizeof(USB_interface_descriptor_t),
    .bDescriptorType = USB_DESCRIPTOR_TYPE_INTERFACE,
    .bInterfaceNumber = USBD_UAC_CONTROL_INTERFACE_INDEX,
    .bAlternateSetting = 0,
    .bNumEndpoints = USBD_UAC_CONTROL_ENDPOINT_INDEX_LAST,
    .bInterfaceClass = USB_CLASS_CODE_AUDIO,
    .bInterfaceSubClass = 0,
    .bInterfaceProtocol = 0,
    .iInterface = USBD_UAC_CONTROL_INTERFACE_STRING_DESCRIPTOR_INDEX
};

static const USB_interface_descriptor_t USB_UAC_STREAM_PLAY_INTERFACE_DESCRIPTOR = {
    .bLength = sizeof(USB_interface_descriptor_t),
    .bDescriptorType = USB_DESCRIPTOR_TYPE_INTERFACE,
    .bInterfaceNumber = USBD_UAC_STREAM_PLAY_INTERFACE_INDEX,
    .bAlternateSetting = 0,
    .bNumEndpoints = USBD_UAC_STREAM_PLAY_ENDPOINT_INDEX_LAST,
    .bInterfaceClass = USB_CLASS_CODE_AUDIO,
    .bInterfaceSubClass = 0,
    .bInterfaceProtocol = 0,
    .iInterface = USBD_UAC_STREAM_PLAY_INTERFACE_STRING_DESCRIPTOR_INDEX
};

static const USB_interface_descriptor_t USB_UAC_STREAM_RECORD_INTERFACE_DESCRIPTOR = {
    .bLength = sizeof(USB_interface_descriptor_t),
    .bDescriptorType = USB_DESCRIPTOR_TYPE_INTERFACE,
    .bInterfaceNumber = USBD_UAC_STREAM_RECORD_INTERFACE_INDEX,
    .bAlternateSetting = 0,
    .bNumEndpoints = USBD_UAC_STREAM_RECORD_ENDPOINT_INDEX_LAST,
    .bInterfaceClass = USB_CLASS_CODE_AUDIO,
    .bInterfaceSubClass = 0,
    .bInterfaceProtocol = 0,
    .iInterface = USBD_UAC_STREAM_RECORD_INTERFACE_STRING_DESCRIPTOR_INDEX
};

static USBD_UAC_context_t usbd_uac_ctx = {
    .cs_descriptor = { [0 ... (USBD_UAC_CS_DESCRIPTOR_BUFFER_SIZE_BYTES - 1)] = 0x00 },
    .cs_descriptor_length = 0,
};

/*** USB UAC global variables ***/

const USB_interface_t USBD_UAC_CONTROL_INTERFACE = {
    .descriptor = &USB_UAC_CONTROL_INTERFACE_DESCRIPTOR,
    .endpoint_list = (const USB_endpoint_t**) &USBD_UAC_CONTROL_INTERFACE_EP_LIST,
    .number_of_endpoints = USBD_UAC_CONTROL_ENDPOINT_INDEX_LAST,
    .cs_descriptor = (const uint8_t**) &(usbd_uac_ctx.cs_descriptor),
    .cs_descriptor_length = &(usbd_uac_ctx.cs_descriptor_length),
    .request_callback = &_USBD_UAC_CONTROL_request_callback
};

const USB_interface_t USBD_UAC_STREAM_PLAY_INTERFACE = {
    .descriptor = &USB_UAC_STREAM_PLAY_INTERFACE_DESCRIPTOR,
    .endpoint_list = (const USB_endpoint_t**) &USBD_UAC_STREAM_PLAY_INTERFACE_EP_LIST,
    .number_of_endpoints = USBD_UAC_STREAM_PLAY_ENDPOINT_INDEX_LAST,
    .cs_descriptor = NULL,
    .cs_descriptor_length = NULL,
    .request_callback = NULL
};

const USB_interface_t USBD_UAC_STREAM_RECORD_INTERFACE = {
    .descriptor = &USB_UAC_STREAM_RECORD_INTERFACE_DESCRIPTOR,
    .endpoint_list = (const USB_endpoint_t**) &USBD_UAC_STREAM_RECORD_INTERFACE_EP_LIST,
    .number_of_endpoints = USBD_UAC_STREAM_RECORD_ENDPOINT_INDEX_LAST,
    .cs_descriptor = NULL,
    .cs_descriptor_length = NULL,
    .request_callback = NULL
};

/*** USBD UAC local functions ***/

/*******************************************************************/
static USB_status_t _USBD_UAC_CONTROL_request_callback(USB_request_t* request, USB_data_t* data_out, USB_data_t* data_in) {
    // Local variables.
    USB_status_t status = USB_SUCCESS;
    // TODO
    UNUSED(request);
    UNUSED(data_out);
    UNUSED(data_in);
    return status;
}

/*******************************************************************/
static void _USBD_UAC_CONTROL_endpoint_in_callback(void) {
    // TODO
}

/*******************************************************************/
static void _USBD_UAC_STREAM_PLAY_endpoint_out_callback(void) {
    // TODO
}

/*******************************************************************/
static void _USBD_UAC_STREAM_RECORD_endpoint_in_callback(void) {
    // TODO
}

/*** USBD UAC functions ***/

/*******************************************************************/
USB_status_t USBD_UAC_init(USBD_UAC_callbacks_t* uac_callbacks) {
    // Local variables.
    USB_status_t status = USB_SUCCESS;
    uint8_t idx = 0;
    // Check parameter.
    if (uac_callbacks == NULL) {
        status = USB_ERROR_NULL_PARAMETER;
        goto errors;
    }
    // Register callbacks.
    usbd_uac_ctx.callbacks = uac_callbacks;
    // Endpoints loop.
    for (idx = 0; idx < (USBD_UAC_CONTROL_INTERFACE.number_of_endpoints); idx++) {
        // Register endpoint.
        status = USBD_HW_register_endpoint((USB_physical_endpoint_t*) ((USBD_UAC_CONTROL_INTERFACE.endpoint_list)[idx]->physical_endpoint));
        if (status != USB_SUCCESS) goto errors;
    }
    for (idx = 0; idx < (USBD_UAC_STREAM_PLAY_INTERFACE.number_of_endpoints); idx++) {
        // Register endpoint.
        status = USBD_HW_register_endpoint((USB_physical_endpoint_t*) ((USBD_UAC_STREAM_PLAY_INTERFACE.endpoint_list)[idx]->physical_endpoint));
        if (status != USB_SUCCESS) goto errors;
    }
    for (idx = 0; idx < (USBD_UAC_STREAM_RECORD_INTERFACE.number_of_endpoints); idx++) {
        // Register endpoint.
        status = USBD_HW_register_endpoint((USB_physical_endpoint_t*) ((USBD_UAC_STREAM_RECORD_INTERFACE.endpoint_list)[idx]->physical_endpoint));
        if (status != USB_SUCCESS) goto errors;
    }
errors:
    return status;
}

/*******************************************************************/
USB_status_t USBD_UAC_de_init(void) {
    // Local variables.
    USB_status_t status = USB_SUCCESS;
    uint8_t idx = 0;
    // Reset context.
    usbd_uac_ctx.callbacks = NULL;
    // Endpoints loop.
    for (idx = 0; idx < (USBD_UAC_CONTROL_INTERFACE.number_of_endpoints); idx++) {
        // Register endpoint.
        status = USBD_HW_unregister_endpoint((USB_physical_endpoint_t*) ((USBD_UAC_CONTROL_INTERFACE.endpoint_list)[idx]->physical_endpoint));
        if (status != USB_SUCCESS) goto errors;
    }
    for (idx = 0; idx < (USBD_UAC_STREAM_PLAY_INTERFACE.number_of_endpoints); idx++) {
        // Register endpoint.
        status = USBD_HW_unregister_endpoint((USB_physical_endpoint_t*) ((USBD_UAC_STREAM_PLAY_INTERFACE.endpoint_list)[idx]->physical_endpoint));
        if (status != USB_SUCCESS) goto errors;
    }
    for (idx = 0; idx < (USBD_UAC_STREAM_RECORD_INTERFACE.number_of_endpoints); idx++) {
        // Register endpoint.
        status = USBD_HW_unregister_endpoint((USB_physical_endpoint_t*) ((USBD_UAC_STREAM_RECORD_INTERFACE.endpoint_list)[idx]->physical_endpoint));
        if (status != USB_SUCCESS) goto errors;
    }
errors:
    return status;
}

#endif /* USB_LIB_DISABLE */
