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
#include "common/usb_cdc.h"
#include "common/usb_interface.h"
#include "common/usb_types.h"
#include "error.h"
#include "types.h"

#if (!(defined USB_LIB_DISABLE) && (defined USBD_CDC))

/*** USB CDC global structures ***/

/*!******************************************************************
 * \enum USBD_CDC_stop_bits_t
 * \brief USB CDC stop bits configurations list.
 *******************************************************************/
typedef enum {
    USBD_CDC_STOP_BITS_1 = 0,
    USBD_CDC_STOP_BITS_1_5,
    USBD_CDC_STOP_BITS_2,
    USBD_CDC_STOP_BITS_LAST
} USBD_CDC_stop_bits_t;

/*!******************************************************************
 * \enum USBD_CDC_parity_t
 * \brief USB CDC parity modes list.
 *******************************************************************/
typedef enum {
    USBD_CDC_PARITY_NONE = 0,
    USBD_CDC_PARITY_ODD,
    USBD_CDC_PARITY_EVEN,
    USBD_CDC_PARITY_MARK,
    USBD_CDC_PARITY_SPACE,
    USBD_CDC_PARITY_LAST
} USBD_CDC_parity_t;

/*!******************************************************************
 * \struct USB_CDC_serial_port_configuration_t
 * \brief USB CDC serial port parameters structure.
 *******************************************************************/
typedef struct {
    uint32_t baud_rate;
    uint8_t data_bits;
    USBD_CDC_stop_bits_t stop_bits;
    USBD_CDC_parity_t parity;
} USB_CDC_serial_port_configuration_t;

/*!******************************************************************
 * \fn USB_CDC_set_serial_port_configuration_cb_t
 * \brief USBD CDC set serial port configuration request callback.
 *******************************************************************/
typedef USB_status_t (*USB_CDC_set_serial_port_configuration_cb_t)(USB_CDC_serial_port_configuration_t* configuration);

/*!******************************************************************
 * \fn USB_CDC_get_serial_port_configuration_cb_t
 * \brief USBD CDC get serial port configuration request callback.
 *******************************************************************/
typedef USB_status_t (*USB_CDC_get_serial_port_configuration_cb_t)(USB_CDC_serial_port_configuration_t* configuration);

/*!******************************************************************
 * \fn USB_CDC_set_serial_port_state_cb_t
 * \brief USBD CDC send serial port state callback.
 *******************************************************************/
typedef USB_status_t (*USB_CDC_set_serial_port_state_cb_t)(uint8_t rts, uint8_t dtr);

/*!******************************************************************
 * \fn USB_CDC_send_break_cb_t
 * \brief USBD CDC send break request callback.
 *******************************************************************/
typedef USB_status_t (*USB_CDC_send_break_cb_t)(void);

/*!******************************************************************
 * \fn USB_CDC_rx_completion_irq_cb_t
 * \brief USBD CDC data reception completion callback.
 *******************************************************************/
typedef USB_status_t (*USB_CDC_rx_completion_irq_cb_t)(uint8_t data);

/*!******************************************************************
 * \fn USB_CDC_tx_completion_irq_cb_t
 * \brief USBD CDC data transmission completion callback.
 *******************************************************************/
typedef USB_status_t (*USB_CDC_tx_completion_irq_cb_t)(void);

/*!******************************************************************
 * \struct USBD_CDC_callbacks_t
 * \brief USBD CDC driver callbacks.
 *******************************************************************/
typedef struct {
    USB_CDC_set_serial_port_configuration_cb_t set_serial_port_configuration_request;
    USB_CDC_get_serial_port_configuration_cb_t get_serial_port_configuration_request;
    USB_CDC_set_serial_port_state_cb_t set_serial_port_state;
    USB_CDC_send_break_cb_t send_break;
    USB_CDC_rx_completion_irq_cb_t rx_completion;
    USB_CDC_tx_completion_irq_cb_t tx_completion;
} USBD_CDC_callbacks_t;

/*** USB CDC global variables ***/

extern const USB_interface_t USBD_CDC_COMM_INTERFACE;
extern const USB_interface_t USBD_CDC_DATA_INTERFACE;

/*** USB CDC functions ***/

/*!******************************************************************
 * \fn USB_status_t USBD_CDC_init(USBD_CDC_callbacks_t* cdc_callbacks)
 * \brief Init USB device CDC class driver.
 * \param[in]   cdc_callbacks: Pointer to the CDC device class callbacks.
 * \param[out]  none
 * \retval      Function execution status.
 *******************************************************************/
USB_status_t USBD_CDC_init(USBD_CDC_callbacks_t* cdc_callbacks);

/*!******************************************************************
 * \fn USB_status_t USBD_CDC_de_init(void)
 * \brief Release USB device CDC class driver.
 * \param[in]   none
 * \param[out]  none
 * \retval      Function execution status.
 *******************************************************************/
USB_status_t USBD_CDC_de_init(void);

/*******************************************************************/
#define USBD_CDC_exit_error(base) { ERROR_check_exit(usbd_cdc_status, USBD_CDC_SUCCESS, base) }

/*******************************************************************/
#define USBD_CDC_stack_error(base) { ERROR_check_stack(usbd_cdc_status, USBD_CDC_SUCCESS, base) }

/*******************************************************************/
#define USBD_CDC_stack_exit_error(base, code) { ERROR_check_stack_exit(usbd_cdc_status, USBD_CDC_SUCCESS, base, code) }

#endif /* USB_LIB_DISABLE */

#endif /* __USBD_CDC_H__ */
