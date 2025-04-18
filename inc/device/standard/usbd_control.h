/*
 * usbd_control.h
 *
 *  Created on: 22 apr. 2023
 *      Author: Ludo
 */

#ifndef __USBD_CONTROL_H__
#define __USBD_CONTROL_H__

#ifndef USB_LIB_DISABLE_FLAGS_FILE
#include "usb_lib_flags.h"
#endif
#include "common/usb_device.h"
#include "common/usb_endpoint.h"
#include "common/usb_interface.h"
#include "common/usb_types.h"
#include "error.h"
#include "strings.h"
#include "types.h"

/*** USBD CONTROL structures ***/

/*!******************************************************************
 * \enum USBD_CONTROL_status_t
 * \brief USBD CONTROL driver error codes.
 *******************************************************************/
typedef enum {
    // Driver errors.
    USBD_CONTROL_SUCCESS = 0,
    USBD_CONTROL_ERROR_NULL_PARAMETER,
    USBD_CONTROL_ERROR_ALREADY_INITIALIZED,
    USBD_CONTROL_ERROR_UNINITIALIZED,
    USBD_CONTROL_ERROR_FULL_CONFIGURATION_DESCRIPTOR_SIZE,
    USBD_CONTROL_ERROR_ENDPOINT_NUMBER,
    USBD_CONTROL_ERROR_ENDPOINT_DIRECTION,
    USBD_CONTROL_ERROR_ENDPOINT_BUFFER_MODE,
    USBD_CONTROL_ERROR_REQUEST_TYPE,
    USBD_CONTROL_ERROR_REQUEST_SIZE,
    USBD_CONTROL_ERROR_STANDARD_REQUEST,
    USBD_CONTROL_ERROR_DESCRIPTOR_TYPE,
    USBD_CONTROL_ERROR_CONFIGURATION_INDEX,
    USBD_CONTROL_ERROR_STRING_DESCRIPTOR_INDEX,
    // Low level drivers errors.
    USBD_CONTROL_ERROR_BASE_HW_INTERFACE = ERROR_BASE_STEP,
    USBD_CONTROL_ERROR_BASE_STRING = (USBD_CONTROL_ERROR_BASE_HW_INTERFACE + USB_LIB_HW_INTERFACE_ERROR_BASE_LAST),
    // Last base value.
    USBD_CONTROL_ERROR_BASE_LAST = (USBD_CONTROL_ERROR_BASE_STRING + STRING_ERROR_BASE_LAST)
} USBD_CONTROL_status_t;

#ifndef USB_LIB_DISABLE

/*!******************************************************************
 * \fn USB_control_set_configuration_cb_t
 * \brief USBD CONTROL set configuration request callback.
 *******************************************************************/
typedef USBD_CONTROL_status_t (*USB_control_set_configuration_cb_t)(uint8_t index);

/*!******************************************************************
 * \enum USBD_CONTROL_requests_callbacks_t
 * \brief USBD CONTROL standard requests callbacks.
 *******************************************************************/
typedef struct {
    USB_control_set_configuration_cb_t set_configuration;
} USBD_CONTROL_requests_callbacks_t;

/*** USBD CONTROL global variables ***/

extern const USB_interface_t USBD_CONTROL_INTERFACE;

/*** USB functions ***/

/*!******************************************************************
 * \fn USBD_CONTROL_status_t USBD_CONTROL_init(USBD_CONTROL_requests_callbacks_t* requests_callbacks)
 * \brief Init USB device control pipe driver.
 * \param[in]   requests_callbacks: Pointer to the standard requests callbacks.
 * \param[out]  none
 * \retval      Function execution status.
 *******************************************************************/
USBD_CONTROL_status_t USBD_CONTROL_init(const USB_device_t* device, USBD_CONTROL_requests_callbacks_t* requests_callbacks);

/*!******************************************************************
 * \fn USBD_CONTROL_status_t USBD_CONTROL_de_init(void)
 * \brief Release USB device control pipe driver.
 * \param[in]   none
 * \param[out]  none
 * \retval      Function execution status.
 *******************************************************************/
USBD_CONTROL_status_t USBD_CONTROL_de_init(void);

/*******************************************************************/
#define USBD_CONTROL_exit_error(base) { ERROR_check_exit(usbd_control_status, USBD_CONTROL_SUCCESS, base) }

/*******************************************************************/
#define USBD_CONTROL_stack_error(base) { ERROR_check_stack(usbd_control_status, USBD_CONTROL_SUCCESS, base) }

/*******************************************************************/
#define USBD_CONTROL_stack_exit_error(base, code) { ERROR_check_stack_exit(usbd_control_status, USBD_CONTROL_SUCCESS, base, code) }

#endif /* USB_LIB_DISABLE */

#endif /* __USB_H__ */
