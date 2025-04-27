/*
 * usb_uac.h
 *
 *  Created on: 26 apr. 2025
 *      Author: Ludo
 */

#ifndef __USB_UAC_H__
#define __USB_UAC_H__

/*** USB UAC structures ***/

/*!******************************************************************
 * \enum USB_UAC_subclass_code_t
 * \brief USB UAC sub-class codes list.
 *******************************************************************/
typedef enum {
    USB_UAC_SUBCLASS_CODE_UNDEFINED = 0x00,
    USB_UAC_SUBCLASS_CODE_AUDIO_CONTROL = 0x01,
    USB_UAC_SUBCLASS_CODE_AUDIO_STREAMING = 0x02,
    USB_UAC_SUBCLASS_CODE_MIDI_STREAMING = 0x03
} USB_UAC_subclass_code_t;

/*!******************************************************************
 * \enum USB_UAC_class_protocol_code_t
 * \brief USB UAC class protocol codes list.
 *******************************************************************/
typedef enum {
    USB_UAC_PROTOCOL_CODE_UNDEFINED = 0x00,
    USB_UAC_PROTOCOL_CODE_IP_VERSION_02_00 = 0x20
} USB_UAC_protocol_code_t;


#endif /* __USB_UAC_H__ */
