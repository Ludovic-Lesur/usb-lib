/*
 * usbd_uac.h
 *
 *  Created on: 26 apr. 2025
 *      Author: Ludo
 */

#ifndef __USBD_UAC_H__
#define __USBD_UAC_H__

#ifndef USB_LIB_DISABLE_FLAGS_FILE
#include "usb_lib_flags.h"
#endif
#include "common/usb_interface.h"
#include "common/usb_types.h"
#include "common/usb_uac.h"
#include "error.h"
#include "types.h"

#if (!(defined USB_LIB_DISABLE) && (defined USBD_UAC))

/*** USBD UAC structures ***/

/*!******************************************************************
 * \struct USBD_UAC_callbacks_t
 * \brief USBD UAC driver callbacks.
 *******************************************************************/
typedef struct {
    // TODO
} USBD_UAC_callbacks_t;

/*** USBD UAC global variables ***/

extern const USB_interface_t USBD_UAC_CONTROL_INTERFACE;
extern const USB_interface_t USBD_UAC_STREAM_PLAY_INTERFACE;
extern const USB_interface_t USBD_UAC_STREAM_RECORD_INTERFACE;

/*** USBD UAC functions ***/

/*!******************************************************************
 * \fn USB_status_t USBD_UAC_init(USBD_UAC_callbacks_t* uac_callbacks)
 * \brief Init USB device audio class driver.
 * \param[in]   uac_callbacks: Pointer to the UAC device class callbacks.
 * \param[out]  none
 * \retval      Function execution status.
 *******************************************************************/
USB_status_t USBD_UAC_init(USBD_UAC_callbacks_t* uac_callbacks);

/*!******************************************************************
 * \fn USB_status_t USBD_UAC_de_init(void)
 * \brief Release USB device audio class driver.
 * \param[in]   none
 * \param[out]  none
 * \retval      Function execution status.
 *******************************************************************/
USB_status_t USBD_UAC_de_init(void);

#endif /* USB_LIB_DISABLE */

#endif /* __USBD_UAC_H__ */
