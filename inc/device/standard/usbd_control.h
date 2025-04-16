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
#include "error.h"
#include "types.h"
#include "types/usb_device.h"
#include "types/usb_endpoint.h"
#include "types/usb_interface.h"
#include "types/usb_types.h"

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
    USBD_CONTROL_ERROR_ENDPOINT_NUMBER,
    USBD_CONTROL_ERROR_ENDPOINT_DIRECTION,
    USBD_CONTROL_ERROR_ENDPOINT_BUFFER_MODE,
    USBD_CONTROL_ERROR_STANDARD_REQUEST_SIZE,
    USBD_CONTROL_ERROR_BREQUEST,
    USBD_CONTROL_ERROR_DESCRIPTOR_TYPE,
    // Low level drivers errors.
    USBD_CONTROL_ERROR_BASE_HW_INTERFACE = ERROR_BASE_STEP,
    // Last base value.
    USBD_CONTROL_ERROR_BASE_LAST = (USBD_CONTROL_ERROR_BASE_HW_INTERFACE + USB_LIB_HW_INTERFACE_ERROR_BASE_LAST)
} USBD_CONTROL_status_t;

#ifndef USB_LIB_DISABLE

/*!******************************************************************
 * \fn USB_control_get_descriptor_cb_t
 * \brief USBD CONTROL get descriptor request callback.
 *******************************************************************/
typedef USBD_CONTROL_status_t (*USB_control_get_descriptor_cb_t)(USB_descriptor_type_t type, uint8_t index, uint8_t** descriptor_ptr, uint32_t* descriptor_size_bytes);

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
    USB_control_get_descriptor_cb_t get_descriptor;
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
USBD_CONTROL_status_t USBD_CONTROL_init(USBD_CONTROL_requests_callbacks_t* requests_callbacks);

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
