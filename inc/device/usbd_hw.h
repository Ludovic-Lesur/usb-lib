/*
 * usbd_hw.h
 *
 *  Created on: 08 apr. 2025
 *      Author: Ludo
 */

#ifndef __USBD_HW_H__
#define __USBD_HW_H__

#include "device/usbd.h"
#include "types/usb_endpoint.h"
#include "types/usb_types.h"

#ifndef USB_LIB_DISABLE

/*** USBD HW functions ***/

/*!******************************************************************
 * \fn USBD_status_t USBD_HW_init(void)
 * \brief Init USB peripheral.
 * \param[in]   none
 * \param[out]  none
 * \retval      Function execution status.
 *******************************************************************/
USBD_status_t USBD_HW_init(void);

/*!******************************************************************
 * \fn USBD_status_t USBD_HW_de_init(void)
 * \brief Release USB peripheral.
 * \param[in]   none
 * \param[out]  none
 * \retval      Function execution status.
 *******************************************************************/
USBD_status_t USBD_HW_de_init(void);

/*!******************************************************************
 * \fn USBD_status_t USBD_HW_register_setup_callback(USB_setup_cb_t setup_callback)
 * \brief Register specific setup callback.
 * \param[in]   setup_callback: Function to call on setup event.
 * \param[out]  none
 * \retval      Function execution status.
 *******************************************************************/
USBD_status_t USBD_HW_register_setup_callback(USB_setup_cb_t setup_callback);

/*!******************************************************************
 * \fn USBD_status_t USBD_HW_register_endpoint(USB_physical_endpoint_t* endpoint)
 * \brief Register end-point in the USB peripheral.
 * \param[in]   endpoint: Pointer to the physical endpoint to register.
 * \param[out]  none
 * \retval      Function execution status.
 *******************************************************************/
USBD_status_t USBD_HW_register_endpoint(USB_physical_endpoint_t* endpoint);

/*!******************************************************************
 * \fn USBD_status_t USBD_HW_unregister_endpoint(USB_physical_endpoint_t* endpoint)
 * \brief Unregister end-point in the USB peripheral.
 * \param[in]   endpoint: Pointer to the physical endpoint to remove.
 * \param[out]  none
 * \retval      Function execution status.
 *******************************************************************/
USBD_status_t USBD_HW_unregister_endpoint(USB_physical_endpoint_t* endpoint);

/*!******************************************************************
 * \fn USBD_status_t USBD_HW_set_address(uint8_t device_address)
 * \brief Set USB device address.
 * \param[in]   device_address: Address to set.
 * \param[out]  none
 * \retval      Function execution status.
 *******************************************************************/
USBD_status_t USBD_HW_set_address(uint8_t device_address);

/*!******************************************************************
 * \fn USBD_status_t USBD_HW_start(void)
 * \brief Start USB peripheral.
 * \param[in]   none
 * \param[out]  none
 * \retval      Function execution status.
 *******************************************************************/
USBD_status_t USBD_HW_start(void);

/*!******************************************************************
 * \fn USBD_status_t USBD_HW_stop(void)
 * \brief Stop USB peripheral.
 * \param[in]   none
 * \param[out]  none
 * \retval      Function execution status.
 *******************************************************************/
USBD_status_t USBD_HW_stop(void);

/*!******************************************************************
 * \fn USBD_status_t USBD_HW_write(USB_physical_endpoint_t* endpoint, USB_data_t* usb_data_in)
 * \brief Write data to USB bus.
 * \param[in]   endpoint: Pointer to the physical endpoint to use.
 * \param[in]   usb_data_in: Pointer to the data to write.
 * \param[out]  none
 * \retval      Function execution status.
 *******************************************************************/
USBD_status_t USBD_HW_write(USB_physical_endpoint_t* endpoint, USB_data_t* usb_data_in);

/*!******************************************************************
 * \fn USBD_status_t USBD_HW_read(USB_physical_endpoint_t* endpoint, USB_data_t* usb_data_out)
 * \brief Read data from USB bus.
 * \param[in]   endpoint: Pointer to the physical endpoint to use.
 * \param[in]   usb_data_out: Pointer to the data to read.
 * \param[out]  none
 * \retval      Function execution status.
 *******************************************************************/
USBD_status_t USBD_HW_read(USB_physical_endpoint_t* endpoint, USB_data_t* usb_data_out);

#endif /* USB_LIB_DISABLE */

#endif /* __USBD_HW_H__ */
