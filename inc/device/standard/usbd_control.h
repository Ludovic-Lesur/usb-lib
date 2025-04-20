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
#include "common/usb_request.h"
#include "common/usb_types.h"
#include "error.h"
#include "strings.h"
#include "types.h"

/*** USBD CONTROL structures ***/

#ifndef USB_LIB_DISABLE

/*!******************************************************************
 * \fn USB_control_set_configuration_cb_t
 * \brief USBD CONTROL set configuration request callback.
 *******************************************************************/
typedef USB_status_t (*USB_control_set_configuration_cb_t)(uint8_t index);

/*!******************************************************************
 * \enum USBD_CONTROL_callbacks_t
 * \brief USBD CONTROL driver callbacks.
 *******************************************************************/
typedef struct {
    USB_control_set_configuration_cb_t set_configuration_request;
    USB_request_cb_t vendor_request;
} USBD_CONTROL_callbacks_t;

/*** USBD CONTROL global variables ***/

extern const USB_interface_t USBD_CONTROL_INTERFACE;

/*** USB functions ***/

/*!******************************************************************
 * \fn USB_status_t USBD_CONTROL_init(const USB_device_t* device, USBD_CONTROL_callbacks_t* control_callbacks)
 * \brief Init USB device control pipe driver.
 * \param[in]   device: Pointer to the USB device structure.
 * \param[in]   control_callbacks: Pointer to the control pipe callbacks.
 * \param[out]  none
 * \retval      Function execution status.
 *******************************************************************/
USB_status_t USBD_CONTROL_init(const USB_device_t* device, USBD_CONTROL_callbacks_t* control_callbacks);

/*!******************************************************************
 * \fn USB_status_t USBD_CONTROL_de_init(void)
 * \brief Release USB device control pipe driver.
 * \param[in]   none
 * \param[out]  none
 * \retval      Function execution status.
 *******************************************************************/
USB_status_t USBD_CONTROL_de_init(void);

#endif /* USB_LIB_DISABLE */

#endif /* __USB_H__ */
