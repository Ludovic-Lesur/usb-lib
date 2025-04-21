/*
 * usbd_hw.h
 *
 *  Created on: 08 apr. 2025
 *      Author: Ludo
 */

#ifndef __USBD_HW_H__
#define __USBD_HW_H__

#include "common/usb_endpoint.h"
#include "common/usb_types.h"

#ifndef USB_LIB_DISABLE

/*** USBD HW functions ***/

/*!******************************************************************
 * \fn USB_status_t USBD_HW_init(void)
 * \brief Init USB peripheral.
 * \param[in]   none
 * \param[out]  none
 * \retval      Function execution status.
 *******************************************************************/
USB_status_t USBD_HW_init(void);

/*!******************************************************************
 * \fn USB_status_t USBD_HW_de_init(void)
 * \brief Release USB peripheral.
 * \param[in]   none
 * \param[out]  none
 * \retval      Function execution status.
 *******************************************************************/
USB_status_t USBD_HW_de_init(void);

/*!******************************************************************
 * \fn USB_status_t USBD_HW_register_setup_callback(USB_setup_cb_t setup_callback)
 * \brief Register specific setup callback.
 * \param[in]   setup_callback: Function to call on setup event.
 * \param[out]  none
 * \retval      Function execution status.
 *******************************************************************/
USB_status_t USBD_HW_register_setup_callback(USB_setup_cb_t setup_callback);

/*!******************************************************************
 * \fn USB_status_t USBD_HW_register_endpoint(USB_physical_endpoint_t* endpoint)
 * \brief Register end-point in the USB peripheral.
 * \param[in]   endpoint: Pointer to the physical endpoint to register.
 * \param[out]  none
 * \retval      Function execution status.
 *******************************************************************/
USB_status_t USBD_HW_register_endpoint(USB_physical_endpoint_t* endpoint);

/*!******************************************************************
 * \fn USB_status_t USBD_HW_unregister_endpoint(USB_physical_endpoint_t* endpoint)
 * \brief Unregister end-point in the USB peripheral.
 * \param[in]   endpoint: Pointer to the physical endpoint to remove.
 * \param[out]  none
 * \retval      Function execution status.
 *******************************************************************/
USB_status_t USBD_HW_unregister_endpoint(USB_physical_endpoint_t* endpoint);

/*!******************************************************************
 * \fn USB_status_t USBD_HW_set_address(uint8_t device_address)
 * \brief Set USB device address.
 * \param[in]   device_address: Address to set.
 * \param[out]  none
 * \retval      Function execution status.
 *******************************************************************/
USB_status_t USBD_HW_set_address(uint8_t device_address);

/*!******************************************************************
 * \fn USB_status_t USBD_HW_start(void)
 * \brief Start USB peripheral.
 * \param[in]   none
 * \param[out]  none
 * \retval      Function execution status.
 *******************************************************************/
USB_status_t USBD_HW_start(void);

/*!******************************************************************
 * \fn USB_status_t USBD_HW_stop(void)
 * \brief Stop USB peripheral.
 * \param[in]   none
 * \param[out]  none
 * \retval      Function execution status.
 *******************************************************************/
USB_status_t USBD_HW_stop(void);

/*!******************************************************************
 * \fn USB_status_t USBD_HW_write_data(USB_physical_endpoint_t* endpoint, USB_data_t* usb_data_in)
 * \brief Write data bytes to USB bus.
 * \param[in]   endpoint: Pointer to the physical endpoint to use.
 * \param[in]   usb_data_in: Pointer to the data to write.
 * \param[out]  none
 * \retval      Function execution status.
 *******************************************************************/
USB_status_t USBD_HW_write_data(USB_physical_endpoint_t* endpoint, USB_data_t* usb_data_in);

/*!******************************************************************
 * \fn USB_status_t USBD_HW_read_data(USB_physical_endpoint_t* endpoint, USB_data_t* usb_data_out)
 * \brief Read data bytes from USB bus.
 * \param[in]   endpoint: Pointer to the physical endpoint to use.
 * \param[out]  usb_data_out: Pointer to the data to read.
 * \param[out]  none
 * \retval      Function execution status.
 *******************************************************************/
USB_status_t USBD_HW_read_data(USB_physical_endpoint_t* endpoint, USB_data_t* usb_data_out);

/*!******************************************************************
 * \fn USB_status_t USBD_HW_read_setup(USB_data_t* usb_data_out)
 * \brief Read setup bytes from USB bus control pipe.
 * \param[in]   none
 * \param[out]  usb_setup_out: Pointer to the setup bytes to read.
 * \retval      Function execution status.
 *******************************************************************/
USB_status_t USBD_HW_read_setup(USB_data_t* usb_setup_out);

#endif /* USB_LIB_DISABLE */

#endif /* __USBD_HW_H__ */
