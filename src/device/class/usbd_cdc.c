/*
 * usb_cdc.c
 *
 *  Created on: 14 apr. 2025
 *      Author: Ludo
 */

#include "device/class/usbd_cdc.h"

#ifndef USB_LIB_DISABLE_FLAGS_FILE
#include "usb_lib_flags.h"
#endif
#include "common/usb_cdc.h"
#include "common/usb_device.h"
#include "common/usb_endpoint.h"
#include "common/usb_interface.h"
#include "common/usb_request.h"
#include "common/usb_types.h"
#include "device/usbd_hw.h"
#include "types.h"

#if (!(defined USB_LIB_DISABLE) && (defined USBD_CDC))

/*** USBD CDC local macros ***/

#define USBD_CDC_CS_DESCRIPTOR_BUFFER_SIZE_BYTES    256
#define USBD_CDC_CS_DESCRIPTOR_LENGTH_INDEX         0

/*** USBD CDC local functions declaration ***/

static void _USBD_CDC_COMM_endpoint_in_callback(void);
static void _USBD_CDC_DATA_endpoint_out_callback(void);
static void _USBD_CDC_DATA_endpoint_in_callback(void);

static USB_status_t _USBD_CDC_COMM_request_callback(USB_request_t* request, USB_data_t* data_in);
static USB_status_t _USBD_CDC_DATA_request_callback(USB_request_t* request, USB_data_t* data_in);

/*** USBD CDC local structures ***/

/*******************************************************************/
typedef enum {
    USBD_CDC_COMM_ENDPOINT_INDEX_IN = 0,
    USBD_CDC_COMM_ENDPOINT_INDEX_LAST
} USBD_CDC_com_endpoint_index_t;

/*******************************************************************/
typedef enum {
    USBD_CDC_DATA_ENDPOINT_INDEX_OUT = 0,
    USBD_CDC_DATA_ENDPOINT_INDEX_IN,
    USBD_CDC_DATA_ENDPOINT_INDEX_LAST
} USBD_CDC_data_endpoint_index_t;

/*******************************************************************/
typedef struct {
    uint8_t cs_descriptor[USBD_CDC_CS_DESCRIPTOR_BUFFER_SIZE_BYTES];
    uint8_t cs_descriptor_length;
} USBD_CDC_context_t;

/*** USB CDC local global variables ***/

static const USB_physical_endpoint_t USBD_CDC_EP_PHY_COM_IN = {
    .number = USBD_CDC_COMM_ENDPOINT_NUMBER,
    .direction = USB_ENDPOINT_DIRECTION_IN,
    .transfer_type = USB_ENDPOINT_TRANSFER_TYPE_INTERRUPT,
    .synchronization_type = USB_ENDPOINT_SYNCHRONIZATION_TYPE_NONE,
    .usage_type = USB_ENDPOINT_USAGE_TYPE_DATA,
    .max_packet_size_bytes = USBD_CDC_COMM_PACKET_SIZE_BYTES,
    .callback = &_USBD_CDC_COMM_endpoint_in_callback
};

static const USB_physical_endpoint_t USBD_CDC_EP_PHY_DATA_OUT = {
    .number = USBD_CDC_DATA_ENDPOINT_NUMBER,
    .direction = USB_ENDPOINT_DIRECTION_OUT,
    .transfer_type = USB_ENDPOINT_TRANSFER_TYPE_BULK,
    .synchronization_type = USB_ENDPOINT_SYNCHRONIZATION_TYPE_NONE,
    .usage_type = USB_ENDPOINT_USAGE_TYPE_DATA,
    .max_packet_size_bytes = USBD_CDC_DATA_PACKET_SIZE_BYTES,
    .callback = &_USBD_CDC_DATA_endpoint_out_callback
};

static const USB_physical_endpoint_t USBD_CDC_EP_PHY_DATA_IN = {
    .number = USBD_CDC_DATA_ENDPOINT_NUMBER,
    .direction = USB_ENDPOINT_DIRECTION_IN,
    .transfer_type = USB_ENDPOINT_TRANSFER_TYPE_BULK,
    .synchronization_type = USB_ENDPOINT_SYNCHRONIZATION_TYPE_NONE,
    .usage_type = USB_ENDPOINT_USAGE_TYPE_DATA,
    .max_packet_size_bytes = USBD_CDC_DATA_PACKET_SIZE_BYTES,
    .callback = &_USBD_CDC_DATA_endpoint_in_callback
};

static const USB_endpoint_descriptor_t USBD_CDC_EP_PHY_COM_IN_DESCRIPTOR = {
    .bLength = sizeof(USB_endpoint_descriptor_t),
    .bDescriptorType = USB_DESCRIPTOR_TYPE_ENDPOINT,
    .bEndpointAddress.number = USBD_CDC_EP_PHY_COM_IN.number,
    .bEndpointAddress.direction = USBD_CDC_EP_PHY_COM_IN.direction,
    .bEndpointAddress.reserved_6_4 = 0,
    .bmAttributes.transfer_type = USBD_CDC_EP_PHY_COM_IN.transfer_type,
    .bmAttributes.synchronization_type = USBD_CDC_EP_PHY_COM_IN.synchronization_type,
    .bmAttributes.usage_type = USBD_CDC_EP_PHY_COM_IN.usage_type,
    .bmAttributes.reserved_7_6 = 0,
    .wMaxPacketSize.max_packet_size = USBD_CDC_EP_PHY_COM_IN.max_packet_size_bytes,
    .wMaxPacketSize.transaction_per_microframe = 0,
    .wMaxPacketSize.reserved_15_13 = 0,
    .bInterval = 255
};

static const USB_endpoint_descriptor_t USBD_CDC_EP_PHY_DATA_OUT_DESCRIPTOR = {
    .bLength = sizeof(USB_endpoint_descriptor_t),
    .bDescriptorType = USB_DESCRIPTOR_TYPE_ENDPOINT,
    .bEndpointAddress.number = USBD_CDC_EP_PHY_DATA_OUT.number,
    .bEndpointAddress.direction = USBD_CDC_EP_PHY_DATA_OUT.direction,
    .bEndpointAddress.reserved_6_4 = 0,
    .bmAttributes.transfer_type = USBD_CDC_EP_PHY_DATA_OUT.transfer_type,
    .bmAttributes.synchronization_type = USBD_CDC_EP_PHY_DATA_OUT.synchronization_type,
    .bmAttributes.usage_type = USBD_CDC_EP_PHY_DATA_OUT.usage_type,
    .bmAttributes.reserved_7_6 = 0,
    .wMaxPacketSize.max_packet_size = USBD_CDC_EP_PHY_DATA_OUT.max_packet_size_bytes,
    .wMaxPacketSize.transaction_per_microframe = 0,
    .wMaxPacketSize.reserved_15_13 = 0,
    .bInterval = 1
};

static const USB_endpoint_descriptor_t USBD_CDC_EP_PHY_DATA_IN_DESCRIPTOR = {
    .bLength = sizeof(USB_endpoint_descriptor_t),
    .bDescriptorType = USB_DESCRIPTOR_TYPE_ENDPOINT,
    .bEndpointAddress.number = USBD_CDC_EP_PHY_DATA_IN.number,
    .bEndpointAddress.direction = USBD_CDC_EP_PHY_DATA_IN.direction,
    .bEndpointAddress.reserved_6_4 = 0,
    .bmAttributes.transfer_type = USBD_CDC_EP_PHY_DATA_IN.transfer_type,
    .bmAttributes.synchronization_type = USBD_CDC_EP_PHY_DATA_IN.synchronization_type,
    .bmAttributes.usage_type = USBD_CDC_EP_PHY_DATA_IN.usage_type,
    .bmAttributes.reserved_7_6 = 0,
    .wMaxPacketSize.max_packet_size = USBD_CDC_EP_PHY_DATA_IN.max_packet_size_bytes,
    .wMaxPacketSize.transaction_per_microframe = 0,
    .wMaxPacketSize.reserved_15_13 = 0,
    .bInterval = 1
};

static const USB_endpoint_t USBD_CDC_EP_COM_IN = {
    .physical_endpoint = &USBD_CDC_EP_PHY_COM_IN,
    .descriptor = &USBD_CDC_EP_PHY_COM_IN_DESCRIPTOR
};

static const USB_endpoint_t USBD_CDC_EP_DATA_OUT = {
    .physical_endpoint = &USBD_CDC_EP_PHY_DATA_OUT,
    .descriptor = &USBD_CDC_EP_PHY_DATA_OUT_DESCRIPTOR
};

static const USB_endpoint_t USBD_CDC_EP_DATA_IN = {
    .physical_endpoint = &USBD_CDC_EP_PHY_DATA_IN,
    .descriptor = &USBD_CDC_EP_PHY_DATA_IN_DESCRIPTOR
};

static const USB_endpoint_t* const USBD_CDC_COMM_INTERFACE_EP_LIST[USBD_CDC_COMM_ENDPOINT_INDEX_LAST] = {
    &USBD_CDC_EP_COM_IN
};

static const USB_endpoint_t* const USBD_CDC_DATA_INTERFACE_EP_LIST[USBD_CDC_DATA_ENDPOINT_INDEX_LAST] = {
    &USBD_CDC_EP_DATA_OUT,
    &USBD_CDC_EP_DATA_IN
};

static const USB_interface_descriptor_t USB_CDC_COMM_INTERFACE_DESCRIPTOR = {
    .bLength = sizeof(USB_interface_descriptor_t),
    .bDescriptorType = USB_DESCRIPTOR_TYPE_INTERFACE,
    .bInterfaceNumber = USBD_CDC_COMM_INTERFACE_INDEX,
    .bAlternateSetting = 0,
    .bNumEndpoints = USBD_CDC_COMM_ENDPOINT_INDEX_LAST,
    .bInterfaceClass = USB_CLASS_CODE_CDC_CONTROL,
    .bInterfaceSubClass = USB_CDC_SUBCLASS_CODE_ABSTRACT,
    .bInterfaceProtocol = USB_CDC_PROTOCOL_CODE_NONE,
    .iInterface = USBD_CDC_COMM_INTERFACE_STRING_DESCRIPTOR_INDEX
};

static const USB_interface_descriptor_t USB_CDC_DATA_INTERFACE_DESCRIPTOR = {
    .bLength = sizeof(USB_interface_descriptor_t),
    .bDescriptorType = USB_DESCRIPTOR_TYPE_INTERFACE,
    .bInterfaceNumber = USBD_CDC_DATA_INTERFACE_INDEX,
    .bAlternateSetting = 0,
    .bNumEndpoints = USBD_CDC_DATA_ENDPOINT_INDEX_LAST,
    .bInterfaceClass = USB_CLASS_CODE_CDC_DATA,
    .bInterfaceSubClass = 0,
    .bInterfaceProtocol = 0,
    .iInterface = USBD_CDC_DATA_INTERFACE_STRING_DESCRIPTOR_INDEX
};

static const USB_CDC_header_descriptor_t USB_CDC_HEADER_DESCRIPTOR = {
    .bFunctionLength = sizeof(USB_CDC_header_descriptor_t),
    .bDescriptorType = USB_DESCRIPTOR_TYPE_CLASS_SPECIFIC_INTERFACE,
    .bDescriptorSubtype = USB_CDC_DESCRIPTOR_SUBTYPE_HEADER,
    .bcdCDC = USB_CDC_DESCRIPTOR_VERSION
};

static const USB_CDC_call_descriptor_t USB_CDC_CALL_DESCRIPTOR = {
    .bFunctionLength = sizeof(USB_CDC_call_descriptor_t),
    .bDescriptorType = USB_DESCRIPTOR_TYPE_CLASS_SPECIFIC_INTERFACE,
    .bDescriptorSubtype = USB_CDC_DESCRIPTOR_SUBTYPE_CALL,
    .bmCapabilities.value = 0x01,
    .bDataInterface = USBD_CDC_DATA_INTERFACE_INDEX
};

static const USB_CDC_abstract_descriptor_t USB_CDC_ABSTRACT_DESCRIPTOR = {
    .bFunctionLength = sizeof(USB_CDC_abstract_descriptor_t),
    .bDescriptorType = USB_DESCRIPTOR_TYPE_CLASS_SPECIFIC_INTERFACE,
    .bDescriptorSubtype = USB_CDC_DESCRIPTOR_SUBTYPE_ABSTRACT,
    .bmCapabilities.value = 0x06
};

static const USB_CDC_union_descriptor_t USB_CDC_UNION_DESCRIPTOR = {
    .bFunctionLength = sizeof(USB_CDC_union_descriptor_t),
    .bDescriptorType = USB_DESCRIPTOR_TYPE_CLASS_SPECIFIC_INTERFACE,
    .bDescriptorSubtype = USB_CDC_DESCRIPTOR_SUBTYPE_UNION,
    .bControlInterface = USBD_CDC_COMM_INTERFACE_INDEX,
    .bSubordinateInterface = USBD_CDC_DATA_INTERFACE_INDEX
};

static const uint8_t* const USB_CDC_DESCRIPTOR_LIST[] = {
    (uint8_t*) &USB_CDC_HEADER_DESCRIPTOR,
    (uint8_t*) &USB_CDC_CALL_DESCRIPTOR,
    (uint8_t*) &USB_CDC_ABSTRACT_DESCRIPTOR,
    (uint8_t*) &USB_CDC_UNION_DESCRIPTOR
};

static USBD_CDC_context_t usbd_cdc_ctx = {
    .cs_descriptor = { [0 ... (USBD_CDC_CS_DESCRIPTOR_BUFFER_SIZE_BYTES - 1)] = 0x00 },
    .cs_descriptor_length = 0
};

/*** USB CDC global variables ***/

const USB_interface_t USBD_CDC_COMM_INTERFACE = {
    .descriptor = &USB_CDC_COMM_INTERFACE_DESCRIPTOR,
    .endpoint_list = (const USB_endpoint_t**) &USBD_CDC_COMM_INTERFACE_EP_LIST,
    .number_of_endpoints = USBD_CDC_COMM_ENDPOINT_INDEX_LAST,
    .cs_descriptor = (const uint8_t**) &(usbd_cdc_ctx.cs_descriptor),
    .cs_descriptor_length = &(usbd_cdc_ctx.cs_descriptor_length),
    .request_callback = &_USBD_CDC_COMM_request_callback
};

const USB_interface_t USBD_CDC_DATA_INTERFACE = {
    .descriptor = &USB_CDC_DATA_INTERFACE_DESCRIPTOR,
    .endpoint_list = (const USB_endpoint_t**) &USBD_CDC_DATA_INTERFACE_EP_LIST,
    .number_of_endpoints = USBD_CDC_DATA_ENDPOINT_INDEX_LAST,
    .cs_descriptor = NULL,
    .cs_descriptor_length = NULL,
    .request_callback = &_USBD_CDC_DATA_request_callback
};

/*** USB CDC local functions ***/

/*******************************************************************/
static USB_status_t _USBD_CDC_COMM_request_callback(USB_request_t* request, USB_data_t* data_in) {
    // Local variables.
    USB_status_t status = USB_SUCCESS;
    // TODO
    UNUSED(request);
    UNUSED(data_in);
    return status;
}

/*******************************************************************/
static USB_status_t _USBD_CDC_DATA_request_callback(USB_request_t* request, USB_data_t* data_in) {
    // Local variables.
    USB_status_t status = USB_SUCCESS;
    // TODO
    UNUSED(request);
    UNUSED(data_in);
    return status;
}

/*******************************************************************/
static void _USBD_CDC_COMM_endpoint_in_callback(void) {
    // TODO
}

/*******************************************************************/
static void _USBD_CDC_DATA_endpoint_out_callback(void) {
    // TODO
}

/*******************************************************************/
static void _USBD_CDC_DATA_endpoint_in_callback(void) {
    // TODO
}

/*** USBD CDC functions ***/

/*******************************************************************/
USB_status_t USBD_CDC_init(void) {
    // Local variables.
    USB_status_t status = USB_SUCCESS;
    const uint8_t* descriptor_ptr = NULL;
    uint32_t descriptor_idx = 0;
    uint32_t full_idx = 0;
    uint32_t idx = 0;
    // Build class specific descriptor.
    for (descriptor_idx = 0; descriptor_idx < (sizeof(USB_CDC_DESCRIPTOR_LIST) / (sizeof(uint8_t*))); descriptor_idx++) {
        // Update pointer.
        descriptor_ptr = USB_CDC_DESCRIPTOR_LIST[descriptor_idx];
        // Bytes loop.
        for (idx = 0; idx < descriptor_ptr[USBD_CDC_CS_DESCRIPTOR_LENGTH_INDEX] ; idx++) {
            // Copy descriptor.
            usbd_cdc_ctx.cs_descriptor[full_idx++] = descriptor_ptr[idx];
            // Check length.
            if (full_idx >= USBD_CDC_CS_DESCRIPTOR_BUFFER_SIZE_BYTES) {
                status = USB_ERROR_CS_DESCRIPTOR_SIZE;
                goto errors;
            }
        }
    }
    usbd_cdc_ctx.cs_descriptor_length = full_idx;
    // Endpoints loop.
    for (idx = 0; idx < (USBD_CDC_COMM_INTERFACE.number_of_endpoints); idx++) {
        // Register endpoint.
        status = USBD_HW_register_endpoint((USB_physical_endpoint_t*) ((USBD_CDC_COMM_INTERFACE.endpoint_list)[idx]->physical_endpoint));
        if (status != USB_SUCCESS) goto errors;
    }
    for (idx = 0; idx < (USBD_CDC_DATA_INTERFACE.number_of_endpoints); idx++) {
        // Register endpoint.
        status = USBD_HW_register_endpoint((USB_physical_endpoint_t*) ((USBD_CDC_DATA_INTERFACE.endpoint_list)[idx]->physical_endpoint));
        if (status != USB_SUCCESS) goto errors;
    }
errors:
    return status;
}

/*******************************************************************/
USB_status_t USBD_CDC_de_init(void) {
    // Local variables.
    USB_status_t status = USB_SUCCESS;
    // Registers endpoints.
    uint8_t idx = 0;
    // Endpoints loop.
    for (idx = 0; idx < (USBD_CDC_COMM_INTERFACE.number_of_endpoints); idx++) {
        // Register endpoint.
        status = USBD_HW_unregister_endpoint((USB_physical_endpoint_t*) ((USBD_CDC_COMM_INTERFACE.endpoint_list)[idx]->physical_endpoint));
        if (status != USB_SUCCESS) goto errors;
    }
    for (idx = 0; idx < (USBD_CDC_DATA_INTERFACE.number_of_endpoints); idx++) {
        // Register endpoint.
        status = USBD_HW_unregister_endpoint((USB_physical_endpoint_t*) ((USBD_CDC_DATA_INTERFACE.endpoint_list)[idx]->physical_endpoint));
        if (status != USB_SUCCESS) goto errors;
    }
errors:
    return status;
}

#endif /* USB_LIB_DISABLE */
