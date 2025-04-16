/*
 * usbd.h
 *
 *  Created on: 08 apr. 2025
 *      Author: Ludo
 */

#ifndef __USBD_H__
#define __USBD_H__

#ifndef USB_LIB_DISABLE_FLAGS_FILE
#include "usb_lib_flags.h"
#endif
#include "error.h"

/*** USBD structures ***/

/*!******************************************************************
 * \enum USBD_status_t
 * \brief USBD driver error codes.
 *******************************************************************/
typedef enum {
    // Driver errors.
    USBD_SUCCESS = 0,
    USBD_ERROR_ALREADY_INITIALIZED,
    USBD_ERROR_UNINITIALIZED,
    // Low level drivers errors.
    USBD_ERROR_BASE_HW_INTERFACE = ERROR_BASE_STEP,
    // Last base value.
    USBD_ERROR_BASE_LAST = (USBD_ERROR_BASE_HW_INTERFACE + USB_LIB_HW_INTERFACE_ERROR_BASE_LAST)
} USBD_status_t;

#ifndef USB_LIB_DISABLE

/*** USBD functions ***/

/*!******************************************************************
 * \fn USBD_status_t USBD_init(void)
 * \brief Init USB device library and hardware interface.
 * \param[in]   none
 * \param[out]  none
 * \retval      Function execution status.
 *******************************************************************/
USBD_status_t USBD_init(void);

/*!******************************************************************
 * \fn USBD_status_t USBD_de_init(void)
 * \brief Release USB device library and hardware interface.
 * \param[in]   none
 * \param[out]  none
 * \retval      Function execution status.
 *******************************************************************/
USBD_status_t USBD_de_init(void);

/*!******************************************************************
 * \fn USBD_status_t USBD_start(void)
 * \brief Start USB device library and hardware interface.
 * \param[in]   none
 * \param[out]  none
 * \retval      Function execution status.
 *******************************************************************/
USBD_status_t USBD_start(void);

/*!******************************************************************
 * \fn USBD_status_t USBD_stop(void)
 * \brief Stop USB device library and hardware interface.
 * \param[in]   none
 * \param[out]  none
 * \retval      Function execution status.
 *******************************************************************/
USBD_status_t USBD_stop(void);

/*******************************************************************/
#define USBD_exit_error(base) { ERROR_check_exit(usbd_status, USBD_SUCCESS, base) }

/*******************************************************************/
#define USBD_stack_error(base) { ERROR_check_stack(usbd_status, USBD_SUCCESS, base) }

/*******************************************************************/
#define USBD_stack_exit_error(base, code) { ERROR_check_stack_exit(usbd_status, USBD_SUCCESS, base, code) }

#endif /* USB_LIB_DISABLE */

#endif /* __USBD_H__ */
