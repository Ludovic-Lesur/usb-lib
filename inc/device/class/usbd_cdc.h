/*
 * usbd_cdc.h
 *
 *  Created on: 14 apr. 2025
 *      Author: Ludo
 */

#ifndef __USBD_CDC_H__
#define __USBD_CDC_H__

#ifndef USB_LIB_DISABLE_FLAGS_FILE
#include "usb_lib_flags.h"
#endif
#include "common/usb_interface.h"
#include "common/usb_types.h"
#include "error.h"
#include "types.h"

/*** USB CDC structures ***/

/*!******************************************************************
 * \enum USBD_CDC_status_t
 * \brief USBD CDC driver error codes.
 *******************************************************************/
typedef enum {
    // Driver errors.
    USBD_CDC_SUCCESS = 0,
    // Low level drivers errors.
    USBD_CDC_ERROR_BASE_HW_INTERFACE = ERROR_BASE_STEP,
    // Last base value.
    USBD_CDC_ERROR_BASE_LAST = (USBD_CDC_ERROR_BASE_HW_INTERFACE + USB_LIB_HW_INTERFACE_ERROR_BASE_LAST)
} USBD_CDC_status_t;

#if (!(defined USB_LIB_DISABLE) && (defined USBD_CDC))

/*** USB CDC global variables ***/

extern const USB_interface_t USBD_CDC_COM_INTERFACE;
extern const USB_interface_t USBD_CDC_DATA_INTERFACE;

/*** USB CDC functions ***/

/*!******************************************************************
 * \fn USBD_CDC_status_t USBD_CDC_init(void)
 * \brief Init USB device CDC class driver.
 * \param[in]   none
 * \param[out]  none
 * \retval      Function execution status.
 *******************************************************************/
USBD_CDC_status_t USBD_CDC_init(void);

/*!******************************************************************
 * \fn USBD_CDC_status_t USBD_CDC_de_init(void)
 * \brief Release USB device CDC class driver.
 * \param[in]   none
 * \param[out]  none
 * \retval      Function execution status.
 *******************************************************************/
USBD_CDC_status_t USBD_CDC_de_init(void);

/*******************************************************************/
#define USBD_CDC_exit_error(base) { ERROR_check_exit(usbd_cdc_status, USBD_CDC_SUCCESS, base) }

/*******************************************************************/
#define USBD_CDC_stack_error(base) { ERROR_check_stack(usbd_cdc_status, USBD_CDC_SUCCESS, base) }

/*******************************************************************/
#define USBD_CDC_stack_exit_error(base, code) { ERROR_check_stack_exit(usbd_cdc_status, USBD_CDC_SUCCESS, base, code) }

#endif /* USB_LIB_DISABLE */

#endif /* __USBD_CDC_H__ */
