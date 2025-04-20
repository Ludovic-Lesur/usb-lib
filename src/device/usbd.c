/*
 * usbd.c
 *
 *  Created on: 08 apr. 2025
 *      Author: Ludo
 */

#include "device/usbd.h"

#include "common/usb_types.h"
#include "device/usbd_hw.h"

#ifndef USB_LIB_DISABLE

/*** USBD local structures ***/

/*******************************************************************/
typedef union {
    struct {
        uint8_t init :1;
    };
    uint8_t all;
} USBD_flags_t;

/*******************************************************************/
typedef struct {
    volatile USBD_flags_t flags;
} USBD_context_t;

/*** USBD local global variables ***/

static USBD_context_t usbd_ctx = {
    .flags.all = 0
};

/*** USBD functions ***/

/*******************************************************************/
USB_status_t USBD_init(void) {
    // Local variables.
    USB_status_t status = USB_SUCCESS;
    // Check state.
    if (usbd_ctx.flags.init != 0) {
        status = USB_ERROR_ALREADY_INITIALIZED;
        goto errors;
    }
    // Init hardware interface.
    status = USBD_HW_init();
    if (status != USB_SUCCESS) goto errors;
    // Init context.
    usbd_ctx.flags.all = 0;
    // Update initialization flag.
    usbd_ctx.flags.init = 1;
errors:
    return status;
}

/*******************************************************************/
USB_status_t USBD_de_init(void) {
    // Local variables.
    USB_status_t status = USB_SUCCESS;
    // Check state.
    if (usbd_ctx.flags.init == 0) {
        status = USB_ERROR_UNINITIALIZED;
        goto errors;
    }
    // Init hardware interface.
    status = USBD_HW_de_init();
    if (status != USB_SUCCESS) goto errors;
    // Update initialization flag.
    usbd_ctx.flags.init = 0;
errors:
    return status;
}

/*******************************************************************/
USB_status_t USBD_start(void) {
    // Local variables.
    USB_status_t status = USB_SUCCESS;
    // Check state.
    if (usbd_ctx.flags.init == 0) {
        status = USB_ERROR_UNINITIALIZED;
        goto errors;
    }
    // Start hardware interface.
    status = USBD_HW_start();
    if (status != USB_SUCCESS) goto errors;
errors:
    return status;
}

/*******************************************************************/
USB_status_t USBD_stop(void) {
    // Local variables.
    USB_status_t status = USB_SUCCESS;
    // Check state.
    if (usbd_ctx.flags.init == 0) {
        status = USB_ERROR_UNINITIALIZED;
        goto errors;
    }
    // Stop hardware interface.
    status = USBD_HW_stop();
    if (status != USB_SUCCESS) goto errors;
errors:
    return status;
}

#endif /* USB_LIB_DISABLE */
