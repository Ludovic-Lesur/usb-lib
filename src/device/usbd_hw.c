/*
 * usbd_hw.c
 *
 *  Created on: 08 apr. 2025
 *      Author: Ludo
 */

#include "device/usbd_hw.h"

#include "device/usbd.h"
#include "types/usb_endpoint.h"
#include "types.h"

#ifndef USB_LIB_DISABLE

/*** USBD HW functions ***/

/*******************************************************************/
USBD_status_t __attribute__((weak)) USBD_HW_init(void) {
    // Local variables.
    USBD_status_t status = USBD_SUCCESS;
    /* To be implemented */
    return status;
}

/*******************************************************************/
USBD_status_t __attribute__((weak)) USBD_HW_de_init(void) {
    // Local variables.
    USBD_status_t status = USBD_SUCCESS;
    /* To be implemented */
    return status;
}

/*******************************************************************/
USBD_status_t __attribute__((weak)) USBD_HW_register_setup_callback(USB_setup_cb_t setup_callback) {
    // Local variables.
    USBD_status_t status = USBD_SUCCESS;
    /* To be implemented */
    UNUSED(setup_callback);
    return status;
}

/*******************************************************************/
USBD_status_t __attribute__((weak)) USBD_HW_register_endpoint(USB_physical_endpoint_t* endpoint) {
    // Local variables.
    USBD_status_t status = USBD_SUCCESS;
    /* To be implemented */
    UNUSED(endpoint);
    return status;
}

/*******************************************************************/
USBD_status_t __attribute__((weak)) USBD_HW_unregister_endpoint(USB_physical_endpoint_t* endpoint) {
    // Local variables.
    USBD_status_t status = USBD_SUCCESS;
    /* To be implemented */
    UNUSED(endpoint);
    return status;
}

/*******************************************************************/
USBD_status_t __attribute__((weak)) USBD_HW_set_address(uint8_t device_address) {
    // Local variables.
    USBD_status_t status = USBD_SUCCESS;
    /* To be implemented */
    UNUSED(device_address);
    return status;
}

/*******************************************************************/
USBD_status_t __attribute__((weak)) USBD_HW_start(void) {
    // Local variables.
    USBD_status_t status = USBD_SUCCESS;
    /* To be implemented */
    return status;
}

/*******************************************************************/
USBD_status_t __attribute__((weak)) USBD_HW_stop(void) {
    // Local variables.
    USBD_status_t status = USBD_SUCCESS;
    /* To be implemented */
    return status;
}

/*******************************************************************/
USBD_status_t __attribute__((weak)) USBD_HW_write(USB_physical_endpoint_t* endpoint, USB_data_t* usb_data_in) {
    // Local variables.
    USBD_status_t status = USBD_SUCCESS;
    /* To be implemented */
    UNUSED(endpoint);
    UNUSED(usb_data_in);
    return status;
}

/*******************************************************************/
USBD_status_t __attribute__((weak)) USBD_HW_read(USB_physical_endpoint_t* endpoint, USB_data_t* usb_data_out) {
    // Local variables.
    USBD_status_t status = USBD_SUCCESS;
    /* To be implemented */
    UNUSED(endpoint);
    UNUSED(usb_data_out);
    return status;
}

#endif /* USB_LIB_DISABLE */
