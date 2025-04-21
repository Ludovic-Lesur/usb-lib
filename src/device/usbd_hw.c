/*
 * usbd_hw.c
 *
 *  Created on: 08 apr. 2025
 *      Author: Ludo
 */

#include "device/usbd_hw.h"

#include "common/usb_endpoint.h"
#include "common/usb_types.h"
#include "types.h"

#ifndef USB_LIB_DISABLE

/*** USBD HW functions ***/

/*******************************************************************/
USB_status_t __attribute__((weak)) USBD_HW_init(void) {
    // Local variables.
    USB_status_t status = USB_SUCCESS;
    /* To be implemented */
    return status;
}

/*******************************************************************/
USB_status_t __attribute__((weak)) USBD_HW_de_init(void) {
    // Local variables.
    USB_status_t status = USB_SUCCESS;
    /* To be implemented */
    return status;
}

/*******************************************************************/
USB_status_t __attribute__((weak)) USBD_HW_register_setup_callback(USB_setup_cb_t setup_callback) {
    // Local variables.
    USB_status_t status = USB_SUCCESS;
    /* To be implemented */
    UNUSED(setup_callback);
    return status;
}

/*******************************************************************/
USB_status_t __attribute__((weak)) USBD_HW_register_endpoint(USB_physical_endpoint_t* endpoint) {
    // Local variables.
    USB_status_t status = USB_SUCCESS;
    /* To be implemented */
    UNUSED(endpoint);
    return status;
}

/*******************************************************************/
USB_status_t __attribute__((weak)) USBD_HW_unregister_endpoint(USB_physical_endpoint_t* endpoint) {
    // Local variables.
    USB_status_t status = USB_SUCCESS;
    /* To be implemented */
    UNUSED(endpoint);
    return status;
}

/*******************************************************************/
USB_status_t __attribute__((weak)) USBD_HW_set_address(uint8_t device_address) {
    // Local variables.
    USB_status_t status = USB_SUCCESS;
    /* To be implemented */
    UNUSED(device_address);
    return status;
}

/*******************************************************************/
USB_status_t __attribute__((weak)) USBD_HW_start(void) {
    // Local variables.
    USB_status_t status = USB_SUCCESS;
    /* To be implemented */
    return status;
}

/*******************************************************************/
USB_status_t __attribute__((weak)) USBD_HW_stop(void) {
    // Local variables.
    USB_status_t status = USB_SUCCESS;
    /* To be implemented */
    return status;
}

/*******************************************************************/
USB_status_t __attribute__((weak)) USBD_HW_write_data(USB_physical_endpoint_t* endpoint, USB_data_t* usb_data_in) {
    // Local variables.
    USB_status_t status = USB_SUCCESS;
    /* To be implemented */
    UNUSED(endpoint);
    UNUSED(usb_data_in);
    return status;
}

/*******************************************************************/
USB_status_t __attribute__((weak)) USBD_HW_read_data(USB_physical_endpoint_t* endpoint, USB_data_t* usb_data_out) {
    // Local variables.
    USB_status_t status = USB_SUCCESS;
    /* To be implemented */
    UNUSED(endpoint);
    UNUSED(usb_data_out);
    return status;
}

USB_status_t __attribute__((weak)) USBD_HW_read_setup(USB_data_t* usb_setup_out) {
    // Local variables.
    USB_status_t status = USB_SUCCESS;
    /* To be implemented */
    UNUSED(usb_setup_out);
    return status;
}

#endif /* USB_LIB_DISABLE */
