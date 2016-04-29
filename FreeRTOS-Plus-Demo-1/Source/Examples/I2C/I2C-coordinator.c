/*
    FreeRTOS V7.3.0 - Copyright (C) 2012 Real Time Engineers Ltd.


    ***************************************************************************
     *                                                                       *
     *    FreeRTOS tutorial books are available in pdf and paperback.        *
     *    Complete, revised, and edited pdf reference manuals are also       *
     *    available.                                                         *
     *                                                                       *
     *    Purchasing FreeRTOS documentation will not only help you, by       *
     *    ensuring you get running as quickly as possible and with an        *
     *    in-depth knowledge of how to use FreeRTOS, it will also help       *
     *    the FreeRTOS project to continue with its mission of providing     *
     *    professional grade, cross platform, de facto standard solutions    *
     *    for microcontrollers - completely free of charge!                  *
     *                                                                       *
     *    >>> See http://www.FreeRTOS.org/Documentation for details. <<<     *
     *                                                                       *
     *    Thank you for using FreeRTOS, and thank you for your support!      *
     *                                                                       *
    ***************************************************************************


    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation AND MODIFIED BY the FreeRTOS exception.
    >>>NOTE<<< The modification to the GPL is included to allow you to
    distribute a combined work that includes FreeRTOS without being obliged to
    provide the source code for proprietary components outside of the FreeRTOS
    kernel.  FreeRTOS is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
    or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details. You should have received a copy of the GNU General Public
    License and the FreeRTOS license exception along with FreeRTOS; if not it
    can be viewed here: http://www.freertos.org/a00114.html and also obtained
    by writing to Richard Barry, contact details for whom are available on the
    FreeRTOS WEB site.

    1 tab == 4 spaces!

    http://www.FreeRTOS.org - Documentation, latest information, license and
    contact details.

    http://www.SafeRTOS.com - A version that is certified for use in safety
    critical systems.

    http://www.OpenRTOS.com - Commercial support, development, porting,
    licensing and training services.
*/
//
//lecture - starting file for I2C examples - need to explore and add comments to related headers
//          and source files, as needed ???
//
//
//lecture - freertos header files
//
/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
//
//
//lecture - freertos+IO header file - also, explore the nested header files, as needed ??
//
//
/* FreeRTO+IO includes. */
#include "FreeRTOS_IO.h"

//
//lecture - related firmware driver header file ???
//
/* Library includes. */
//#include "lpc17xx_gpio.h"  //aaa-zzz
//
//lecture - headers related to examples ???
//
/* Example includes. */
#include "I2C-coordinator.h"
#include "I2C-to-OLED.h"
#include "I2C-to-and-from-EEPROM.h"
#include "oled.h"

/*-----------------------------------------------------------*/

/* Place holder for calls to ioctl that don't use the value parameter. */
#define i2cPARAMETER_NOT_USED			( ( void * ) 0 )

/* The size, in bytes, of the circular buffer used when the I2C port is using
circular buffer Rx mode. */
#define i2cCIRCULAR_BUFFER_SIZE			( ( void * ) 50 )

/* The maximum amount of time to wait to receive the requested number of bytes
when using zero copy receive mode. */
#define i2c200MS_TIMEOUT				( ( void * ) ( 200UL / portTICK_RATE_MS ) )
/*-----------------------------------------------------------*/

/*
 * The task that uses the I2C to communicate with the OLED.
 */
static void prvI2CTask( void *pvParameters );

/*-----------------------------------------------------------*/

//
//lecture - file that will be handling I2C related operations
//
//
//lecture - i2c-eeprom - this is the task created to manage i2c related activities
//
//lecture - i2c-eeprom - refer to the task code prvI2CTask, in this source file
//
//
void vI2CTaskStart( void )
{
	/* Just create the I2C task, then return.  The I2C task communicates with
	the OLED and the EEPROM. */
	xTaskCreate( 	prvI2CTask,								/* The task that uses the I2C peripheral. */
					( const int8_t * const ) "I2CO",	/* Text name assigned to the task.  This is just to assist debugging.  The kernel does not use this name itself. */
					configI2C_TASK_STACK_SIZE,				/* The size of the stack allocated to the task. */
					NULL,									/* The parameter is not used, so NULL is passed. */
					configI2C_TASK_PRIORITY,				/* The priority allocated to the task. */
					NULL );									/* A handle to the task being created is not required, so just pass in NULL. */
}
/*-----------------------------------------------------------*/
//
//
//
//
//lecture - i2c-eeprom - mostly hw issues and freertos+IO
//
//
//lecture - i2c-eeprom - we must open the I2C2 port - the I2C master controller -
//                             refer to LPCXpresso17xx-base-board.h - the BSP file
//
//lecture - i2c-eeprom - the same descriptor(of master controller) is used to
//          access slaves on the given bus ???
//
//
//lecture - i2c-eeprom - we need to refer to i2c-to-and-from-eeprom.c for details and code
//
//
//
//
//
static void prvI2CTask( void *pvParameters )
{
Peripheral_Descriptor_t xI2CPort;
const uint32_t ulMaxDelay = 500UL / portTICK_RATE_MS;

	( void ) pvParameters;
	//
    //
	//lecture - this call is connected to FreeRTOS+IO and related device/driver
	//
	//lecture - what is the default tx mode and rx mode ? interrupt or polling
	//          what is the buffering technique used for tx and rx ?? polling tx and polling rx
	//lecture - refer to freertos+io webpages for more details
	//
    //
	//lecture - i2c-eeprom - i2c2 master is opened for accessing slave - i2c2-eeprom is
	//          is the slave that we wish to access
	//lecture - i2c-eeprom - refer to LPCXpresso17xx-base-board.h, in FreeRTOS+IO
	//          component - it contains the name of the device to be opened -
	//          in this case, boardOLED_I2C_PORT (??) - i2c2 master
	//
	//lecture - i2c-eeprom - if the open is successful, we get a handle to
	//          the active device descriptor - we can use this handle to
	//          access the slave devices connected on this I2C2 bus, as needed
	//
	//lecture - i2c-eeprom - refer to lpc17xx user-manual for more details
	//          on i2c2 port and its characteristics
	//
	//lecture - i2c-eeprom - by default, when an active device is set-up,
	//          the tx mode is set to polling and rx mode is set to polling
	//
	//lecture - refer to freertos.org links(??) for tx polling mode and rx polling mode
	//          operations, related rules and usage of APIs
	//
	//lecture - eventually, create a customized code for i2c-eeprom separately and test it(??)
	//
	//
	//lecture - i2c-eeprom - FreeRTOS_open() is a freertos+io API - refer to
	//          common/FreeRTOS_DriverInterface.c of freertos+io component
	//
	//lecture - i2c-eeprom - FreeRTOS_open() runs very deep - follow the
	//                       comments in common/FreeRTOS_DriverInterface.c
	//
	//
	/* Open the I2C port used for writing to both the OLED and the EEPROM.  The
	second parameter (ulFlags) is not used in this case.  The port is opened in
	polling mode.  It is changed to interrupt driven mode later in this
	function. */
	xI2CPort = FreeRTOS_open( boardEEPROM_I2C_PORT, ( uint32_t ) i2cPARAMETER_NOT_USED );
	configASSERT( xI2CPort );

	//lecture - refer to I2C-to-OLED.c for specific methods

	/* The OLED must be initialised before it is used. */
//	vI2C_OLEDInitialise( xI2CPort );   //aaa-zzz

	//lecture - refer to oled.c for more details - most routines here are to manage
	//          oled IO - explore meticulously ???

	/* Write and read-back operations are to be performed on the EEPROM while
	the I2C bus is in polling mode.  Indicate this on the OLED. */
//	vOLEDPutString( 0U, ( uint8_t * ) "Testing EEPROM", OLED_COLOR_WHITE, OLED_COLOR_BLACK );
//	vOLEDPutString( oledCHARACTER_HEIGHT, ( uint8_t * ) "in polling mode", OLED_COLOR_WHITE, OLED_COLOR_BLACK );
//	vOLEDRefreshDisplay();    //aaa-zzz
    //
	//
	//lecture - i2c-eeprom - refer to I2C-to-from-EEPROM.c for more details and code, as needed -
	//          I2C based IO transfer is being done with eeprom on the specific I2C bus - i2c2 !!!
	//
	//lecture - i2c-eeprom - initial test is performed with tx mode in polling and rx mode in polling
	//
	//lecture - i2c-eeprom - refer to the website links mentioned below :
	//          http://www.freertos.org/FreeRTOS-Plus/FreeRTOS_Plus_IO/Polled_Transfer_Mode.shtml
	//
	//lecture - also, refer to freertos+io APIs
	//
	//lecture - i2c-eeprom - we need to go deep into the code used for
	//                       writing and reading eeprom device ?? refer to I2C-to-from-EEPROM.c
	//
	/* Perform the polling mode EEPROM tests/examples. */
	vI2C_EEPROMTest( xI2CPort );
    //
	//lecture - refer to I2C-to-OLED.c for more details - also, refer to hw details ??
	//
	/* Perform the polling mode OLED write example. */
//	vI2C_OLEDTest( xI2CPort, ( uint8_t * ) "in polling mode" );  //aaa-zzz
    //

	//
	//
	//lecture - need to refer to freertos+io webpages for more details on ioctl commands
	//          for switching to interrupt mode Tx(zero copy) and interrupt mode Rx
	//                                                            (circular mode)
	//
	//lecture - add comments, with freertos+io frame-work/code ??
	//
	//lecture - i2c-eeprom - refer to FreeRTOS_DriverInterface.c for
	//                       FreeRTOS_ioctl() API
	//
	//lecture - i2c-eeprom - we pass the handle to the active device object
	//                       command is used to configure the tx and rx modes -
	//
	//lecture - i2c-eeprom - refer to FreeRTOS_DriverInterface.h for standard
	//                       ioctl commands
	//
	//lecture - i2c-eeprom - refer to FreeRTOS_DriverInterface.c for ioctlUSE_ZERO_COPY_TX
	//                       command and ioctl operation - follow the code into
	//                       freertos+io code and i2c specific code
	//
	//lecture - i2c-eeprom - refer to FreeRTOS_DriverInterface.c for ioctlUSE_CIRCULAR_BUFFER_RX
	//                       command and ioctl operation - follow the code into
	//                       freertos+io code and i2c specific code
	//
	//lecture - i2c-eeprom - refer to FreeRTOS_DriverInterface.c for ioctlUSE_RX_TIMEOUT
	//                       command and ioctl operation - follow the code into
	//                       freertos+io code and i2c specific code
	//
	/* Switch to interrupt driven zero copy Tx mode and interrupt driven
	circular buffer Rx mode (with a limited time out). */
	//FreeRTOS_ioctl( xI2CPort, ioctlUSE_ZERO_COPY_TX, i2cPARAMETER_NOT_USED );
	//FreeRTOS_ioctl( xI2CPort, ioctlUSE_CIRCULAR_BUFFER_RX, i2cCIRCULAR_BUFFER_SIZE );
	//FreeRTOS_ioctl( xI2CPort, ioctlSET_RX_TIMEOUT, i2c200MS_TIMEOUT );
    //
	//
	//
	//lecture - on what basis do we assign interrupt priority - need to check the
	//          configuration and as well as i2c specific ioctl code
	//
	/* By default, the I2C interrupt priority will have been set to
	the lowest possible.  It must be kept at or below
	configMAX_LIBRARY_INTERRUPT_PRIORITY, but can be raised above
	its default priority using a FreeRTOS_ioctl() call with the
	ioctlSET_INTERRUPT_PRIORITY command. */
//	FreeRTOS_ioctl( xI2CPort, ioctlSET_INTERRUPT_PRIORITY, ( void * ) ( configMIN_LIBRARY_INTERRUPT_PRIORITY - 1 ) );
    //
	//
	//
	//lecture - all the operations on slaves are done, in interrupt driven mode ??
	//          need to check the freertos+io code, data structures and drivers ???
	//          need to dig deep ???
	//
	/* Write and read-back operations are to be performed on the EEPROM while
	the I2C bus is in interrupt driven zero copy Tx, and interrupt driven
	circular buffer Rx mode.  Indicate this on the OLED. */
	//vOLEDPutString( 0U, ( uint8_t * ) "Testing EEPROM", OLED_COLOR_WHITE, OLED_COLOR_BLACK );
	//vOLEDPutString( oledCHARACTER_HEIGHT, ( uint8_t * ) "in intrpt mode", OLED_COLOR_WHITE, OLED_COLOR_BLACK );
    //aaa-zzz
	//
	//lecture - here, we need to explore and understand how freertos+io works and relates to
	//          our operations ??? how is the mutex used ??
	//
    //lecture - once again, we must refer to OLED code and dig deep from higher level layer
	//          to lower level layers ???
	//
	/* Using zero copy Tx mode means the write mutex must be obtained prior to
	calling vOLEDRefreshDisplay().  The write mutex is retained when
	vOLEDRefreshDisplay() returns. */
//	FreeRTOS_ioctl( xI2CPort, ioctlOBTAIN_WRITE_MUTEX, ( void * ) ulMaxDelay );
//	vOLEDRefreshDisplay();   //aaa-zzz

	//
	//
	//lecture - this code must be explored, in I2C-to-from-EEPROM.c
	//
	//
	/* Perform the interrupt driven mode EEPROM tests/examples. */
//	vI2C_EEPROMTest( xI2CPort );


	//
	//
	//lecture - once again, we must explore and understand I2C oled code
	//          in I2C-to-OLED.c
	//
	/* Finish off by just continuously writing a scrolling message to the
	OLED. */
	for( ;; )
	{
		//vI2C_OLEDTest( xI2CPort, ( uint8_t * ) "in intrpt mode" );   //aaa-zzz
	}
}
/*-----------------------------------------------------------*/

