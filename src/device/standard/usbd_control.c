/*
 * usb_device.c
 *
 *  Created on: 22 apr. 2023
 *      Author: Ludo
 */

#include "device/standard/usbd_control.h"

#ifndef USB_LIB_DISABLE_FLAGS_FILE
#include "usb_lib_flags.h"
#endif
#include "device/usbd.h"
#include "device/usbd_hw.h"
#include "error.h"
#include "types.h"
#include "types/usb_device.h"
#include "types/usb_descriptor.h"
#include "types/usb_request.h"
#include "types/usb_types.h"

#ifndef USB_LIB_DISABLE

/*** USBD CONTROL local macros ***/

#define USBD_CONTROL_PACKET_SIZE_BYTES  64

/*** USBD CONTROL local functions declaration ***/

static void _USBD_CONTROL_endpoint_out_callback(void);
static void _USBD_CONTROL_endpoint_in_callback(void);

/*** USBD CONTROL local structures ***/

/*******************************************************************/
typedef enum {
    USBD_CONTROL_ENDPOINT_INDEX_OUT = 0,
    USBD_CONTROL_ENDPOINT_INDEX_IN,
    USBD_CONTROL_ENDPOINT_INDEX_LAST
} USBD_CONTROL_endpoint_index_t;

/*******************************************************************/
typedef union {
    struct {
        uint8_t init :1;
    };
    uint8_t all;
} USBD_CONTROL_flags_t;

/*******************************************************************/
typedef struct {
    volatile USBD_CONTROL_flags_t flags;
    USBD_CONTROL_requests_callbacks_t* requests_callbacks;
    USB_data_t data_out;
    USB_data_t data_in;
} USBD_CONTROL_context_t;

/*** USBD CONTROL local global variables ***/

static const USB_physical_endpoint_t USBD_CONTROL_EP_PHY_OUT = {
    .number = 0,
    .direction = USB_ENDPOINT_DIRECTION_OUT,
    .transfer_type = USB_ENDPOINT_TRANSFER_TYPE_CONTROL,
    .synchronization_type = USB_ENDPOINT_SYNCHRONIZATION_TYPE_NONE,
    .usage_type = USB_ENDPOINT_USAGE_TYPE_DATA,
    .max_packet_size_bytes = USBD_CONTROL_PACKET_SIZE_BYTES,
    .callback = &_USBD_CONTROL_endpoint_out_callback
};

static const USB_physical_endpoint_t USBD_CONTROL_EP_PHY_IN = {
    .number = 0,
    .direction = USB_ENDPOINT_DIRECTION_IN,
    .transfer_type = USB_ENDPOINT_TRANSFER_TYPE_CONTROL,
    .synchronization_type = USB_ENDPOINT_SYNCHRONIZATION_TYPE_NONE,
    .usage_type = USB_ENDPOINT_USAGE_TYPE_DATA,
    .max_packet_size_bytes = USBD_CONTROL_PACKET_SIZE_BYTES,
    .callback = &_USBD_CONTROL_endpoint_in_callback
};

static const USB_endpoint_descriptor_t USBD_CONTROL_EP_PHY_OUT_DESCRIPTOR = {
    .bLength = sizeof(USB_endpoint_descriptor_t),
    .bDescriptorType = USB_DESCRIPTOR_TYPE_ENDPOINT,
    .bEndpointAddress.number = USBD_CONTROL_EP_PHY_OUT.number,
    .bEndpointAddress.direction = USBD_CONTROL_EP_PHY_OUT.direction,
    .bEndpointAddress.reserved_6_4 = 0,
    .bmAttributes.transfer_type = USBD_CONTROL_EP_PHY_OUT.transfer_type,
    .bmAttributes.synchronization_type = USBD_CONTROL_EP_PHY_OUT.synchronization_type,
    .bmAttributes.usage_type = USBD_CONTROL_EP_PHY_OUT.usage_type,
    .bmAttributes.reserved_7_6 = 0,
    .wMaxPacketSize.max_packet_size = USBD_CONTROL_EP_PHY_OUT.max_packet_size_bytes,
    .wMaxPacketSize.transaction_per_microframe = 0,
    .wMaxPacketSize.reserved_15_13 = 0,
    .bInterval = 1
};

static const USB_endpoint_descriptor_t USBD_CONTROL_EP_PHY_IN_DESCRIPTOR = {
    .bLength = sizeof(USB_endpoint_descriptor_t),
    .bDescriptorType = USB_DESCRIPTOR_TYPE_ENDPOINT,
    .bEndpointAddress.number = USBD_CONTROL_EP_PHY_IN.number,
    .bEndpointAddress.direction = USBD_CONTROL_EP_PHY_IN.direction,
    .bEndpointAddress.reserved_6_4 = 0,
    .bmAttributes.transfer_type = USBD_CONTROL_EP_PHY_IN.transfer_type,
    .bmAttributes.synchronization_type = USBD_CONTROL_EP_PHY_IN.synchronization_type,
    .bmAttributes.usage_type = USBD_CONTROL_EP_PHY_IN.usage_type,
    .bmAttributes.reserved_7_6 = 0,
    .wMaxPacketSize.max_packet_size = USBD_CONTROL_EP_PHY_IN.max_packet_size_bytes,
    .wMaxPacketSize.transaction_per_microframe = 0,
    .wMaxPacketSize.reserved_15_13 = 0,
    .bInterval = 1
};

static const USB_endpoint_t USBD_CONTROL_EP_OUT = {
    .descriptor = &USBD_CONTROL_EP_PHY_OUT_DESCRIPTOR,
    .physical_endpoint = &USBD_CONTROL_EP_PHY_OUT
};

static const USB_endpoint_t USBD_CONTROL_EP_IN = {
    .descriptor = &USBD_CONTROL_EP_PHY_IN_DESCRIPTOR,
    .physical_endpoint = &USBD_CONTROL_EP_PHY_IN
};

static const USB_endpoint_t* const USBD_CONTROL_EP_LIST[USBD_CONTROL_ENDPOINT_INDEX_LAST] = {
    &USBD_CONTROL_EP_OUT,
    &USBD_CONTROL_EP_IN
};

static const USB_interface_descriptor_t USBD_CONTROL_INTERFACE_DESCRIPTOR = {
    .bLength = sizeof(USB_interface_descriptor_t),
    .bDescriptorType = USB_DESCRIPTOR_TYPE_INTERFACE,
    .bInterfaceNumber = USBD_CONTROL_INTERFACE_INDEX,
    .bAlternateSetting = 0,
    .bInterfaceClass = USB_CLASS_CODE_VENDOR_SPECIFIC,
    .bInterfaceSubClass = 0,
    .bInterfaceProtocol = USB_DESCRIPTOR_PROTOCOL_CUSTOM,
    .iInterface = USBD_CONTROL_INTERFACE_STRING_DESCRIPTOR_INDEX
};

static USBD_CONTROL_context_t usbd_control_ctx = {
    .flags.all = 0,
    .requests_callbacks = NULL
};

/*** USBD CONTROL global variables ***/

const USB_interface_t USBD_CONTROL_INTERFACE = {
    .descriptor = &USBD_CONTROL_INTERFACE_DESCRIPTOR,
    .number_of_endpoints = USBD_CONTROL_ENDPOINT_INDEX_LAST,
    .endpoint_list = (const USB_endpoint_t**) &USBD_CONTROL_EP_LIST
};

/*** USBD CONTROL local functions ***/

/*******************************************************************/
static USBD_CONTROL_status_t _USBD_CONTROL_decode_standard_request(USB_data_t* usb_data_out, USB_request_operation_t* request_operation, USB_data_t* usb_data_in) {
    // Local variables.
    USBD_CONTROL_status_t status = USBD_CONTROL_SUCCESS;
    USBD_status_t usbd_status = USBD_SUCCESS;
    USB_request_t* request_packet;
    uint8_t wValue_high = 0;
    uint8_t wValue_low = 0;
    // Check parameters.
    if ((usb_data_out == NULL) || (request_operation == NULL) || ((usb_data_in == NULL))) {
        status = USBD_CONTROL_ERROR_NULL_PARAMETER;
        goto errors;
    }
    // Check data size.
    if ((usb_data_out->size_bytes) < sizeof(USB_request_t)) {
        status = USBD_CONTROL_ERROR_STANDARD_REQUEST_SIZE;
        goto errors;
    }
    // Reset setup request operation.
    (*request_operation) = USB_REQUEST_OPERATION_NOT_SUPPORTED;
    // Reset output data.
    usb_data_in->data = NULL;
    usb_data_in->size_bytes = 0;
    // Cast frame.
    request_packet = (USB_request_t*) (usb_data_out->data);
    // Parse type and index.
    wValue_high = (uint8_t) (((request_packet->wValue) >> 8) & 0xFF);
    wValue_low = (uint8_t) (((request_packet->wValue) >> 0) & 0xFF);
    // Compute transfer type.
    if ((request_packet->wLength) == 0) {
        (*request_operation) = USB_REQUEST_OPERATION_WRITE_NO_DATA;
    }
    else {
        if (((request_packet->bmRequestType).direction) == USB_REQUEST_DIRECTION_HOST_TO_DEVICE) {
            (*request_operation) = USB_REQUEST_OPERATION_WRITE;
        }
        else {
            (*request_operation) = USB_REQUEST_OPERATION_READ;
        }
    }
    // Check request.
    switch (request_packet->bRequest) {
    case USB_REQUEST_ID_GET_DESCRIPTOR:
        // Read descriptor.
        status = usbd_control_ctx.requests_callbacks->get_descriptor(wValue_high, wValue_low, &(usb_data_in->data), &(usb_data_in->size_bytes));
        if (status != USBD_CONTROL_SUCCESS) goto errors;
        break;
    case USB_REQUEST_ID_SET_ADDRESS:
        // Set address on hardware side.
        usbd_status = USBD_HW_set_address(wValue_low);
        USBD_exit_error(USBD_CONTROL_ERROR_BASE_HW_INTERFACE);
        break;
    case USB_REQUEST_ID_SET_CONFIGURATION:
        // Set configuration.
        status = usbd_control_ctx.requests_callbacks->set_configuration(wValue_low);
        if (status != USBD_CONTROL_SUCCESS) goto errors;
        break;
    default:
        // Unsupported request.
        (*request_operation) = USB_REQUEST_OPERATION_NOT_SUPPORTED;
        // Return error.
        status = USBD_CONTROL_ERROR_BREQUEST;
        goto errors;
    }
    // Clamp data size according to request.
    if ((usb_data_in->size_bytes) > (request_packet->wLength)) {
        (usb_data_in->size_bytes) = (request_packet->wLength);
    }
errors:
    return status;
}

/*******************************************************************/
static void _USBD_CONTROL_setup_callback(USB_request_operation_t* setup_request_type) {
    // Local variables.
    USBD_CONTROL_status_t status = USBD_CONTROL_SUCCESS;
    USBD_status_t usbd_status = USBD_SUCCESS;
    // Read data.
    usbd_status = USBD_HW_read((USB_physical_endpoint_t*) &USBD_CONTROL_EP_PHY_OUT, &usbd_control_ctx.data_out);
    USBD_exit_error(USBD_CONTROL_ERROR_BASE_HW_INTERFACE);
    // Decode request.
    status = _USBD_CONTROL_decode_standard_request(&usbd_control_ctx.data_out, setup_request_type, &usbd_control_ctx.data_in);
    if (status != USBD_CONTROL_SUCCESS) goto errors;
    // Write data.
    usbd_status = USBD_HW_write((USB_physical_endpoint_t*) &USBD_CONTROL_EP_PHY_IN, &usbd_control_ctx.data_in);
    USBD_exit_error(USBD_CONTROL_ERROR_BASE_HW_INTERFACE);
errors:
    return;
}

/*******************************************************************/
static void _USBD_CONTROL_endpoint_out_callback(void) {
    // TODO
}

/*******************************************************************/
static void _USBD_CONTROL_endpoint_in_callback(void) {
    // TODO
}

/*** USB functions ***/

/*******************************************************************/
USBD_CONTROL_status_t USBD_CONTROL_init(USBD_CONTROL_requests_callbacks_t* requests_callbacks) {
    // Local variables.
    USBD_CONTROL_status_t status = USBD_CONTROL_SUCCESS;
    USBD_status_t usbd_status = USBD_SUCCESS;
    uint8_t idx = 0;
    // Check parameter.
    if (requests_callbacks == NULL) {
        status = USBD_CONTROL_ERROR_NULL_PARAMETER;
        goto errors;
    }
    // Check state.
    if (usbd_control_ctx.flags.init != 0) {
        status = USBD_CONTROL_ERROR_ALREADY_INITIALIZED;
        goto errors;
    }
    // Reset flags.
    usbd_control_ctx.flags.all = 0;
    // Register callbacks.
    usbd_control_ctx.requests_callbacks = requests_callbacks;
    // Register endpoints.
    for (idx = 0; idx < (USBD_CONTROL_INTERFACE.number_of_endpoints); idx++) {
        usbd_status = USBD_HW_register_endpoint((USB_physical_endpoint_t*) ((USBD_CONTROL_INTERFACE.endpoint_list)[idx]->physical_endpoint));
        USBD_exit_error(USBD_CONTROL_ERROR_BASE_HW_INTERFACE);
    }
    // Register setup callback.
    usbd_status = USBD_HW_register_setup_callback(&_USBD_CONTROL_setup_callback);
    USBD_exit_error(USBD_CONTROL_ERROR_BASE_HW_INTERFACE);
    // Update initialization flag.
    usbd_control_ctx.flags.init = 1;
errors:
    return status;
}

/*******************************************************************/
USBD_CONTROL_status_t USBD_CONTROL_de_init(void) {
    // Local variables.
    USBD_CONTROL_status_t status = USBD_CONTROL_SUCCESS;
    USBD_status_t usbd_status = USBD_SUCCESS;
    uint8_t idx = 0;
    // Check state.
    if (usbd_control_ctx.flags.init == 0) {
        status = USBD_CONTROL_ERROR_UNINITIALIZED;
        goto errors;
    }
    // Reset callbacks.
    usbd_control_ctx.flags.all = 0;
    usbd_control_ctx.requests_callbacks = NULL;
    // Unregister endpoints.
    for (idx = 0; idx < (USBD_CONTROL_INTERFACE.number_of_endpoints); idx++) {
        usbd_status = USBD_HW_unregister_endpoint((USB_physical_endpoint_t*) ((USBD_CONTROL_INTERFACE.endpoint_list)[idx]->physical_endpoint));
        USBD_exit_error(USBD_CONTROL_ERROR_BASE_HW_INTERFACE);
    }
    // Update initialization flag.
    usbd_control_ctx.flags.init = 0;
errors:
    return status;
}

#endif /* USB_LIB_DISABLE */
