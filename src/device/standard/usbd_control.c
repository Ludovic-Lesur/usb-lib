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
#include "common/usb_device.h"
#include "common/usb_descriptor.h"
#include "common/usb_request.h"
#include "common/usb_types.h"
#include "device/usbd.h"
#include "device/usbd_hw.h"
#include "error.h"
#include "strings.h"
#include "types.h"

#ifndef USB_LIB_DISABLE

/*** USBD CONTROL local macros ***/

#define USBD_CONTROL_PACKET_SIZE_BYTES              64

#define USBD_CONTROL_DESCRIPTOR_BUFFER_SIZE_BYTES   1024
#define USBD_CONTROL_DESCRIPTOR_TOTAL_LENGTH_INDEX  2

/*** USBD CONTROL local functions declaration ***/

static void _USBD_CONTROL_endpoint_out_callback(void);
static void _USBD_CONTROL_endpoint_in_callback(void);

static USB_status_t _USBD_CONTROL_standard_request_callback(USB_request_t* request, USB_data_t* data_out, USB_data_t* data_in);

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
        uint8_t in_request_pending :1;
        uint8_t out_request_pending :1;
        uint8_t init :1;
    };
    uint8_t all;
} USBD_CONTROL_flags_t;

/*******************************************************************/
typedef struct {
    volatile USBD_CONTROL_flags_t flags;
    const USB_device_t* device;
    USBD_CONTROL_callbacks_t* callbacks;
    USB_request_operation_t request_operation;
    uint8_t current_configuration_index;
    uint8_t full_configuration_descriptor[USBD_CONTROL_DESCRIPTOR_BUFFER_SIZE_BYTES];
    uint8_t string_descriptor[USBD_CONTROL_DESCRIPTOR_BUFFER_SIZE_BYTES];
    USB_data_t setup_out;
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
    .bInterfaceClass = 0,
    .bInterfaceSubClass = 0,
    .bInterfaceProtocol = 0,
    .iInterface = USBD_CONTROL_INTERFACE_STRING_DESCRIPTOR_INDEX
};

static USBD_CONTROL_context_t usbd_control_ctx = {
    .flags.all = 0,
    .device = NULL,
    .callbacks = NULL,
    .request_operation = USB_REQUEST_OPERATION_NOT_SUPPORTED,
    .current_configuration_index = 0
};

/*** USBD CONTROL global variables ***/

const USB_interface_t USBD_CONTROL_INTERFACE = {
    .descriptor = &USBD_CONTROL_INTERFACE_DESCRIPTOR,
    .number_of_endpoints = USBD_CONTROL_ENDPOINT_INDEX_LAST,
    .endpoint_list = (const USB_endpoint_t**) &USBD_CONTROL_EP_LIST,
    .cs_descriptor = NULL,
    .cs_descriptor_length = NULL,
    .request_callback = &_USBD_CONTROL_standard_request_callback
};

/*** USBD CONTROL local functions ***/

/*******************************************************************/
#define _USBD_CONTROL_full_configuration_descriptor_add_byte(new_byte) { \
    /* Add byte */ \
    usbd_control_ctx.full_configuration_descriptor[full_idx++] = new_byte; \
    /* Check length */ \
    if (full_idx >= USBD_CONTROL_DESCRIPTOR_BUFFER_SIZE_BYTES) { \
        status = USB_ERROR_CONFIGURATION_DESCRIPTOR_SIZE; \
        goto errors; \
    } \
}

/*******************************************************************/
static USB_status_t _USBD_CONTROL_update_configuration_index(uint8_t bConfigurationValue) {
    // Local variables.
    USB_status_t status = USB_ERROR_CONFIGURATION_VALUE;
    uint8_t idx = 0;
    // Configurations loop.
    for (idx = 0; idx < (usbd_control_ctx.device->number_of_configurations); idx++) {
        // Check if configuration number matches.
        if (usbd_control_ctx.device->configuration_list[idx]->descriptor->bConfigurationValue == bConfigurationValue) {
            // Update index.
            usbd_control_ctx.current_configuration_index = idx;
            // Exit loop.
            status = USB_SUCCESS;
            break;
        }
    }
    return status;
}

/*******************************************************************/
static USB_status_t _USBD_CONTROL_build_full_configuration_descriptor(uint8_t index) {
    // Local variables.
    USB_status_t status = USB_SUCCESS;
    const USB_configuration_t* configuration_ptr = NULL;
    const USB_interface_t* interface_ptr = NULL;
    const USB_endpoint_t* endpoint_ptr = NULL;
    uint8_t* descriptor_ptr = NULL;
    uint8_t interface_idx = 0;
    uint8_t endpoint_idx = 0;
    uint32_t full_idx = 0;
    uint32_t idx = 0;
    // Check index.
    if (index >= (usbd_control_ctx.device->number_of_configurations)) {
        status = USB_ERROR_CONFIGURATION_INDEX;
        goto errors;
    }
    // Set configuration pointer.
    configuration_ptr = usbd_control_ctx.device->configuration_list[index];
    // Build full configuration descriptor.
    descriptor_ptr = (uint8_t*) (configuration_ptr->descriptor);
    for (idx = 0; idx < (configuration_ptr->descriptor->bLength); idx++) {
        _USBD_CONTROL_full_configuration_descriptor_add_byte(descriptor_ptr[idx]);
    }
    // Interfaces loop.
    for (interface_idx = (USBD_CONTROL_INTERFACE_INDEX + 1); interface_idx < (configuration_ptr->number_of_interfaces); interface_idx++) {
        // Update pointer.
        interface_ptr = configuration_ptr->interface_list[interface_idx];
        descriptor_ptr = (uint8_t*) (interface_ptr->descriptor);
        // Append interface descriptor.
        for (idx = 0; idx < (interface_ptr->descriptor)->bLength; idx++) {
            _USBD_CONTROL_full_configuration_descriptor_add_byte(descriptor_ptr[idx]);
        }
        // Check class specific descriptors.
        if ((interface_ptr->cs_descriptor != NULL) && (interface_ptr->cs_descriptor_length != NULL)) {
            // Update pointer.
            descriptor_ptr = (uint8_t*) (interface_ptr->cs_descriptor);
            // Append class specific descriptors.
            for (idx = 0; idx < (*(interface_ptr->cs_descriptor_length)); idx++) {
                _USBD_CONTROL_full_configuration_descriptor_add_byte(descriptor_ptr[idx]);
            }
        }
        // Endpoints loop.
        for (endpoint_idx = 0; endpoint_idx < (interface_ptr->number_of_endpoints); endpoint_idx++) {
            // Update pointer.
            endpoint_ptr = interface_ptr->endpoint_list[endpoint_idx];
            descriptor_ptr = (uint8_t*) (endpoint_ptr->descriptor);
            // Append endpoint descriptor.
            for (idx = 0; idx < (endpoint_ptr->descriptor)->bLength; idx++) {
                _USBD_CONTROL_full_configuration_descriptor_add_byte(descriptor_ptr[idx]);
            }
        }
    }
    // Update total length field.
    usbd_control_ctx.full_configuration_descriptor[USBD_CONTROL_DESCRIPTOR_TOTAL_LENGTH_INDEX] = full_idx;
errors:
    return status;
}

/*******************************************************************/
static USB_status_t _USBD_CONTROL_get_descriptor(USB_descriptor_type_t type, uint8_t index, uint8_t** descriptor_ptr, uint32_t* descriptor_size_bytes) {
    // Local variables.
    USB_status_t status = USB_SUCCESS;
    STRING_status_t string_status = STRING_SUCCESS;
    uint32_t str_size = 0;
    uint32_t full_idx = 0;
    uint32_t idx = 0;
    // Reset output.
    (*descriptor_ptr) = NULL;
    (*descriptor_size_bytes) = 0;
    // Check type.
    switch (type) {
    case USB_DESCRIPTOR_TYPE_DEVICE:
        (*descriptor_ptr) = (uint8_t*) (usbd_control_ctx.device->descriptor);
        (*descriptor_size_bytes) = usbd_control_ctx.device->descriptor->bLength;
        break;
    case USB_DESCRIPTOR_TYPE_DEVICE_QUALIFIER:
        (*descriptor_ptr) = (uint8_t*) (usbd_control_ctx.device->qualifier_descriptor);
        (*descriptor_size_bytes) = usbd_control_ctx.device->qualifier_descriptor->bLength;
        break;
    case USB_DESCRIPTOR_TYPE_CONFIGURATION:
    case USB_DESCRIPTOR_TYPE_OTHER_SPEED_CONFIGURATION:
        // Build configuration descriptor.
        status = _USBD_CONTROL_build_full_configuration_descriptor(index);
        if (status != USB_SUCCESS) goto errors;
        // Update pointers.
        (*descriptor_ptr) = (uint8_t*) &(usbd_control_ctx.full_configuration_descriptor);
        (*descriptor_size_bytes) = usbd_control_ctx.full_configuration_descriptor[USBD_CONTROL_DESCRIPTOR_TOTAL_LENGTH_INDEX];
        break;
    case USB_DESCRIPTOR_TYPE_STRING:
        // Check index.
        if (index >= usbd_control_ctx.device->number_of_string_descriptors) {
            status = USB_ERROR_STRING_DESCRIPTOR_INDEX;
            goto errors;
        }
        // Get string length.
        string_status = STRING_get_size((char_t*) (usbd_control_ctx.device->string_descriptor_list[index]), &str_size);
        STRING_exit_error(USB_ERROR_BASE_STRING);
        // Build string descriptor.
        usbd_control_ctx.string_descriptor[full_idx++] = (sizeof(USB_string_descriptor_t) + (str_size << ((index == USB_STRING_DESCRIPTOR_INDEX_LANGID) ? 0 : 1)));
        usbd_control_ctx.string_descriptor[full_idx++] = USB_DESCRIPTOR_TYPE_STRING;
        // Append string.
        for (idx = 0; idx < str_size; idx++) {
            usbd_control_ctx.string_descriptor[full_idx++] = usbd_control_ctx.device->string_descriptor_list[index][idx];
            // Specific case of language ID.
            if (index != USB_STRING_DESCRIPTOR_INDEX_LANGID) {
                usbd_control_ctx.string_descriptor[full_idx++] = 0x00;
            }
        }
        (*descriptor_ptr) = (uint8_t*) &(usbd_control_ctx.string_descriptor);
        (*descriptor_size_bytes) = usbd_control_ctx.string_descriptor[0];
        break;
    default:
        status = USB_ERROR_DESCRIPTOR_TYPE;
        goto errors;
    }
errors:
    return status;
}

/*******************************************************************/
static USB_status_t _USBD_CONTROL_standard_request_callback(USB_request_t* request, USB_data_t* data_out, USB_data_t* data_in) {
    // Local variables.
    USB_status_t status = USB_SUCCESS;
    uint8_t wValue_high = (uint8_t) ((request->wValue >> 8) & 0xFF);
    uint8_t wValue_low = (uint8_t) ((request->wValue >> 0) & 0xFF);
    // Unused parameter.
    UNUSED(data_out);
    // Check request.
    switch (request->bRequest) {
    case USB_REQUEST_GET_DESCRIPTOR:
        // Read descriptor.
        status = _USBD_CONTROL_get_descriptor(wValue_high, wValue_low, &(data_in->data), &(data_in->size_bytes));
        if (status != USB_SUCCESS) goto errors;
        break;
    case USB_REQUEST_SET_ADDRESS:
        // Set address on hardware side.
        status = USBD_HW_set_address(wValue_low);
        if (status != USB_SUCCESS) goto errors;
        break;
    case USB_REQUEST_SET_CONFIGURATION:
        // Set configuration.
        status = usbd_control_ctx.callbacks->set_configuration_request(wValue_low);
        if (status != USB_SUCCESS) goto errors;
        // Update local context.
        status = _USBD_CONTROL_update_configuration_index(wValue_low);
        if (status != USB_SUCCESS) goto errors;
        break;
    default:
        status = USB_ERROR_STANDARD_REQUEST;
        goto errors;
    }
errors:
    return status;
}

/*******************************************************************/
static void _USBD_CONTROL_update_request_operation(void) {
    // Local variables.
    USB_request_t* request_ptr;
    // Reset setup request operation.
    usbd_control_ctx.request_operation = USB_REQUEST_OPERATION_NOT_SUPPORTED;
    // Cast frame.
    request_ptr = (USB_request_t*) (usbd_control_ctx.setup_out.data);
    // Compute transfer type.
    if ((request_ptr->wLength) == 0) {
        usbd_control_ctx.request_operation = USB_REQUEST_OPERATION_WRITE_NO_DATA;
    }
    else {
        if (((request_ptr->bmRequestType).direction) == USB_REQUEST_DIRECTION_HOST_TO_DEVICE) {
            usbd_control_ctx.request_operation = USB_REQUEST_OPERATION_WRITE;
        }
        else {
            usbd_control_ctx.request_operation = USB_REQUEST_OPERATION_READ;
        }
    }
}

/*******************************************************************/
static USB_status_t _USBD_CONTROL_decode_request(void) {
    // Local variables.
    USB_status_t status = USB_SUCCESS;
    const USB_configuration_t* configuration_ptr = NULL;
    const USB_interface_t* interface_ptr = NULL;
    USB_request_t* request_ptr;
    // Check data size.
    if ((usbd_control_ctx.setup_out.size_bytes) < sizeof(USB_request_t)) {
        status = USB_ERROR_REQUEST_SIZE;
        goto errors;
    }
    // Reset output data.
    usbd_control_ctx.data_in.data = NULL;
    usbd_control_ctx.data_in.size_bytes = 0;
    // Cast frame.
    request_ptr = (USB_request_t*) (usbd_control_ctx.setup_out.data);
    // Check type.
    switch (request_ptr->bmRequestType.type) {
    case USB_REQUEST_TYPE_STANDARD:
        // Decode standard request.
        status = _USBD_CONTROL_standard_request_callback(request_ptr, &usbd_control_ctx.data_out, &(usbd_control_ctx.data_in));
        if (status != USB_SUCCESS) goto errors;
        break;
    case USB_REQUEST_TYPE_CLASS:
        // Search corresponding interface.
        configuration_ptr = usbd_control_ctx.device->configuration_list[usbd_control_ctx.current_configuration_index];
        interface_ptr = configuration_ptr->interface_list[request_ptr->wIndex];
        // Check request callback.
        if (interface_ptr->request_callback == NULL) {
            status = USB_ERROR_CLASS_REQUEST;
            goto errors;
        }
        // Execute class specific callback.
        status = interface_ptr->request_callback(request_ptr, &usbd_control_ctx.data_out, &(usbd_control_ctx.data_in));
        if (status != USB_SUCCESS) goto errors;
        break;
    case USB_REQUEST_TYPE_VENDOR:
        // Check vendor callback.
        if (usbd_control_ctx.callbacks->vendor_request == NULL) {
            status = USB_ERROR_VENDOR_REQUEST;
            goto errors;
        }
        // Execute external callback.
        status = usbd_control_ctx.callbacks->vendor_request(request_ptr, &usbd_control_ctx.data_out, &(usbd_control_ctx.data_in));
        if (status != USB_SUCCESS) goto errors;
        break;
    default:
        status = USB_ERROR_REQUEST_TYPE;
        goto errors;
    }
    // Clamp data size according to request.
    if ((usbd_control_ctx.data_in.size_bytes) > (request_ptr->wLength)) {
        usbd_control_ctx.data_in.size_bytes = (request_ptr->wLength);
    }
errors:
    return status;
}

/*******************************************************************/
static USB_status_t _USBD_CONTROL_process_request(void) {
    // Local variables.
    USB_status_t status = USB_SUCCESS;
    // Decode request.
    status = _USBD_CONTROL_decode_request();
    if (status != USB_SUCCESS) goto errors;
    // Check if there is IN data to send.
    if ((usbd_control_ctx.data_in.data != NULL) && (usbd_control_ctx.data_in.size_bytes != 0)) {
        // Send data to host.
        status = USBD_HW_write_data((USB_physical_endpoint_t*) &USBD_CONTROL_EP_PHY_IN, &usbd_control_ctx.data_in);
        if (status != USB_SUCCESS) goto errors;
        // Update flag.
        usbd_control_ctx.flags.in_request_pending = 1;
    }
errors:
    return status;
}

/*******************************************************************/
static void _USBD_CONTROL_setup_callback(USB_request_operation_t* setup_request_type) {
    // Local variables.
    USB_status_t status = USB_SUCCESS;
    // Check parameter.
    if (setup_request_type == NULL) goto errors;
    // Reset request type.
    (*setup_request_type) = USB_REQUEST_OPERATION_NOT_SUPPORTED;
    // Check if there is no pending request.
    if ((usbd_control_ctx.flags.in_request_pending != 0) || (usbd_control_ctx.flags.out_request_pending != 0)) goto errors;
    // Read setup bytes.
    status = USBD_HW_read_setup(&usbd_control_ctx.setup_out);
    if (status != USB_SUCCESS) goto errors;
    // Update request operation.
    _USBD_CONTROL_update_request_operation();
    // Check request type.
    switch (usbd_control_ctx.request_operation) {
    case USB_REQUEST_OPERATION_READ:
    case USB_REQUEST_OPERATION_WRITE_NO_DATA:
        // Request can be processed directly.
        status = _USBD_CONTROL_process_request();
        if (status != USB_SUCCESS) goto errors;
        break;
    case USB_REQUEST_OPERATION_WRITE:
        // Wait for OUT data before processing request.
        usbd_control_ctx.flags.out_request_pending = 1;
        break;
    default:
        break;
    }
    // Update output parameter.
    (*setup_request_type) = usbd_control_ctx.request_operation;
errors:
    return;
}

/*******************************************************************/
static void _USBD_CONTROL_endpoint_out_callback(void) {
    // Local variables.
    USB_status_t status = USB_SUCCESS;
    // Check flag.
    if (usbd_control_ctx.flags.out_request_pending != 0) {
        // Read OUT data bytes.
        status = USBD_HW_read_data((USB_physical_endpoint_t*) &USBD_CONTROL_EP_PHY_OUT, &usbd_control_ctx.data_out);
        if (status != USB_SUCCESS) goto errors;
        // Process request.
        status = _USBD_CONTROL_process_request();
        if (status != USB_SUCCESS) goto errors;
    }
errors:
    // Clear flag.
    usbd_control_ctx.flags.out_request_pending = 0;
    return;
}

/*******************************************************************/
static void _USBD_CONTROL_endpoint_in_callback(void) {
    // Clear flag.
    usbd_control_ctx.flags.in_request_pending = 0;
}

/*** USB functions ***/

/*******************************************************************/
USB_status_t USBD_CONTROL_init(const USB_device_t* device, USBD_CONTROL_callbacks_t* control_callbacks) {
    // Local variables.
    USB_status_t status = USB_SUCCESS;
    uint8_t idx = 0;
    // Check parameter.
    if ((device == NULL) || (control_callbacks == NULL)) {
        status = USB_ERROR_NULL_PARAMETER;
        goto errors;
    }
    // Check state.
    if (usbd_control_ctx.flags.init != 0) {
        status = USB_ERROR_ALREADY_INITIALIZED;
        goto errors;
    }
    // Reset flags.
    usbd_control_ctx.flags.all = 0;
    // Register device and callbacks.
    usbd_control_ctx.device = device;
    usbd_control_ctx.callbacks = control_callbacks;
    // Register endpoints.
    for (idx = 0; idx < (USBD_CONTROL_INTERFACE.number_of_endpoints); idx++) {
        status = USBD_HW_register_endpoint((USB_physical_endpoint_t*) ((USBD_CONTROL_INTERFACE.endpoint_list)[idx]->physical_endpoint));
        if (status != USB_SUCCESS) goto errors;
    }
    // Register setup callback.
    status = USBD_HW_register_setup_callback(&_USBD_CONTROL_setup_callback);
    if (status != USB_SUCCESS) goto errors;
    // Update initialization flag.
    usbd_control_ctx.flags.init = 1;
errors:
    return status;
}

/*******************************************************************/
USB_status_t USBD_CONTROL_de_init(void) {
    // Local variables.
    USB_status_t status = USB_SUCCESS;
    uint8_t idx = 0;
    // Check state.
    if (usbd_control_ctx.flags.init == 0) {
        status = USB_ERROR_UNINITIALIZED;
        goto errors;
    }
    // Reset context.
    usbd_control_ctx.flags.all = 0;
    usbd_control_ctx.device = NULL;
    usbd_control_ctx.callbacks = NULL;
    // Unregister endpoints.
    for (idx = 0; idx < (USBD_CONTROL_INTERFACE.number_of_endpoints); idx++) {
        status = USBD_HW_unregister_endpoint((USB_physical_endpoint_t*) ((USBD_CONTROL_INTERFACE.endpoint_list)[idx]->physical_endpoint));
        if (status != USB_SUCCESS) goto errors;
    }
    // Update initialization flag.
    usbd_control_ctx.flags.init = 0;
errors:
    return status;
}

#endif /* USB_LIB_DISABLE */
