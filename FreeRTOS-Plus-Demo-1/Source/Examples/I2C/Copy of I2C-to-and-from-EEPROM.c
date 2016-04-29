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
//
//lecture - I2C to eeprom operations are commented for more clarity - this code is to be
//          invoked from i2c-coordinator.c
//
//lecture - i2c-eeprom - this file is a copy of the original file
//                       we can use this file to add comments for
//                       tx zero copy mode(interrupt) and rx circular mode(interrupt)
//
//
/* Standard includes. */
#include <string.h>
//
//lecture - refer to freertos headers/nested headers, explore and add comments
//
/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
//
//
//lecture - refer to freertos+IO headers/nested headers, explore and add comments
//
/* FreeRTOS+IO includes. */
#include "FreeRTOS_IO.h"
//
//
//lecture - firmware driver library headers - refer to relevant headers, as per examples
//
/* Library includes. */
// #include "lpc17xx_gpio.h"  //aaa-zzz
//
//
//lecture - refer to relevant example headers
//
//
/* Example includes. */
#include "I2C-to-and-from-EEPROM.h"
//
//
//lecture - i2c-eeprom - hw parameters must be obtained from relevant lpc17xx user-manual,
//          hw manual of eeprom, board user-manual and schematic !!!
//lecture - i2c-eeprom - refer to /mnt4/freertos_IO_bsp_oct2014_/hw_manuals/i2c_eeprom24LC08.pdf
//
//lecture - i2c-eeprom - eeprom capacity is 8kbits - 1024 bytes
//lecture - i2c-eeprom - eeprom page-size is 16bytes - used during write access - for more
//          details refer to eeprom hw manual
//
//lecture - i2c-eeprom
/* EEPROM parameters. */
//#define i2cTOTAL_EEPROM_SIZE 	( 1024UL )  //aaa-zzz
//define i2cPAGE_SIZE			( 16UL )
#define i2cTOTAL_EEPROM_SIZE 	( 32*1024UL )
#define i2cPAGE_SIZE			( 64UL )

//
//
//
//lecture - i2c-eeprom - refer to schematics of EA-baseboard -
//          /mnt4/freertos_IO_bsp_oct2014_/LPCXpresso_Base_Board_revB1.pdf - this provides
//          the allocated slave addresses - 0x50 to 0x53
//
//lecture - i2c-eeprom - slave address can be from 0x50-0x53 - refer to eeprom hw manual for
//          details - refer to section 6.0 of this manual and relevant addressing schemes !!!
//
//
//
/* I2C address of the EEPROM. */
#define i2cEEPROM_SLAVE_ADDRESS	( 0x50UL )
//
//
//lecture - i2c-eeprom - clock frquency for the i2c bus is set to 400khz -
//          refer to lpc17xx user-manual , schematic and eeprom hw manual (??)
//
//
/* Maximum I2C frequency at which the EEPROM can operate at 5V. */
#define i2cEEPROM_CLOCK 		( ( void * ) 400000UL )
//
//
//lecture - i2c-eeprom - refer to table 1-2 of eeprom hw manual - maximum time is 5msecs
//          needed for word write operation or a page write operation to complete - this
//          the information provided by eeprom hw manual !!!
//
//
/* A delay is required to allow the EEPROM to program itself. */
#define i2cWRITE_CYCLE_DELAY	( 5UL )
//
//lecture - i2c-eeprom - refer to the test code ??
//
/* The number of test loops to perform each tie vI2C_EEPROMTest() is called.
Each tests a different page in memory. */
#define i2cEEPROM_TESTS			( 6UL )

/* Place holder for calls to ioctl that don't use the value parameter. */
#define i2cPARAMETER_NOT_USED	( ( void * ) 0 )
//
//
//lecture - i2c-eeprom - we need to find this from the hw details - refer to section 6.0
//          of eeprom hw manual for word addressing - there is a block address(4 blocks) and
//          followed by word address, in a given block (??) - refer to the diagrams provided
//
//
/*
 * The least significant bits of the slave address form part of the EEPROM byte
 * address.  Set the slave address to be correct for the byte address being
 * read from or written to.
 */
static uint8_t prvSetSlaveAddress( Peripheral_Descriptor_t xI2CPort, uint32_t ulByteAddress );
//
//lecture - based on the hw details of eeprom, we can read/write accordingly  - each
//          block can be accessed page by page - a single page is of 16 bytes size ???
//          refer to the section 6.0
//

static void prvWriteBytesToEEPROM( Peripheral_Descriptor_t xI2CPort, uint32_t ulByteAddress );

/*
 * Write an entire page of data to the EEPROM, starting from the byte address
 * specified by the parameter.
 */
static void prvWritePageToEEPROM( Peripheral_Descriptor_t xI2CPort, uint32_t ulByteAddress );

/*-----------------------------------------------------------*/
//
//lecture - we need to find the exact size and value of the eeprom address - from hw details ???
//
//lecture - eeprom - a single bytes is used to store the word address, in a specific block
//
//
/* A buffer large enough to hold a complete page of data plus a byte for the
address of the data. */
static uint8_t ucDataBuffer[ i2cPAGE_SIZE + 2 ];

static uint8_t ucTinyDataBuffer[ 4 + 2 ]={0,0,'A',2,3,'B'}; //aaa-zzz

//
//
//lecture - I2C2 master's handle is opened from i2c-coordinator.c - the handle is passed to
//
//
//lecture - this method is invoked from task created for I2C operations from
//          i2c-coordinator.c
//
//
//
/*-----------------------------------------------------------*/

void vI2C_EEPROMTest( Peripheral_Descriptor_t xI2CPort )
{
uint32_t ulStartAddress = 0UL, ulPage;
const uint32_t ulMaxDelay = 500UL / portTICK_RATE_MS;
static uint8_t ucByte, ucValue;
int32_t lReturned;


//Peripheral_Control_t  pxpheri_ctl = (Peripheral_Control_t *)xI2CPort;
//int i2c_id = pxpheri_ctl->cPeripheralNumber;


    //
    //
    //lecture - i2c port is opened and configured earlier - we may need to
    //          find the configuration here or may need to change the configuration here ???
    //
    //lecture - i2c-eeprom - i2c2 port may be used in polling mode or interrupt mode
    //          i2c--eeprom - we need to code accordingly
    //
    //lecture - i2c-eeprom - in this case,we will using interrupt driven modes
    //
	/* The I2C port is opened and configured in the I2C-coordinator.c file.
	The opened handle is passed in to this file - which just uses the I2C
	FreeRTOS+IO driver with whatever configuration it happens to have at that
	time.  Sometimes it	will be operating in polled mode, and other in
	interrupt driven zero copy Tx with interrupt driven circular buffer Rx. */
    //

    //
    //lecture - i2c-eeprom - what are the hw details involved - refer to lpc17xx user-manual
    //          and eeprom hw manual for more details (??)
    //
    //
    //lecture - i2c-eeprom - initial test is run with Tx polling mode and Rx polling mode
    //          i2c-eeprom - initial comments are for Tx polling mode and Rx polling mode
    //
    //lecture - i2c-eeprom - refer to FreeRTOS_ioctl() from FreeRTOS_DriverInterface.c
    //          i2c-eeprom -
    //
    //lecture - i2c-eeprom - if we invoke ioctl command for mutex operations and current mode
    //                       is tx polling, what happens ?
    //
    //
    //
	/* Set the clock frequency to be correct for the EEPROM. */
	FreeRTOS_ioctl( xI2CPort, ioctlSET_SPEED, i2cEEPROM_CLOCK );
	vPrintString("clockspeed\n");

    //
	//lecture - i2c-eeprom - is this procedure for mutex same for any configuration ???
	//                       mutex operations are effective only for zero copy tx operations -
	//                       otherwise, ineffective ??
	//          i2c-eeprom - refer to FreeRTOS_DriverInterface.c for code and details
	//
	/* The write mutex is obtained in the code below, but it is possible that
	it is already held by this task - in which case the attempt to obtain it
	will fail.  Release the mutex first, just in case. */
	FreeRTOS_ioctl( xI2CPort, ioctlRELEASE_WRITE_MUTEX, i2cPARAMETER_NOT_USED );
    //
	/* Wait until any writes already in progress have completed. */
	FreeRTOS_ioctl( xI2CPort, ioctlOBTAIN_WRITE_MUTEX, ( void * ) ulMaxDelay );


//	prvWriteBytesToEEPROM(xI2CPort, ulStartAddress);


	//
	//
	//lecture - i2c-eeprom - initializing the buffer to 0s, before writing to eeprom
	//
    //
	//
	/* Fill the EEPROM with 0x00, one page at a time. */
	memset( ucDataBuffer, 0x00, sizeof( ucDataBuffer ) );
	//
	//
	//lecture - i2c-eeprom - refer to code, details and comments below, in the same file
	//          i2c-eeprom - refer to prvWritePageToEEPROM() code below
	//
	//lecture - i2c-eeprom - refer to eeprom hw manual - 0x000 to 0x3ff are
	//                       accessed on the eeprom
	//
	//

	for( ulPage = 0UL; ulPage < ( i2cTOTAL_EEPROM_SIZE / i2cPAGE_SIZE ); ulPage++ )
	{
		prvWritePageToEEPROM( xI2CPort, ulStartAddress );

		// Move to the next page.
		ulStartAddress += i2cPAGE_SIZE;

		if(ulPage == 16)
			break;
	}


	/* Check all data read from the EEPROM reads as 0x00.  Start by setting the
	read address back to the start of the EEPROM. */

	ucDataBuffer[ 0 ] = 0x00;
	ucDataBuffer[ 1 ] = 0x00;
	lReturned = FreeRTOS_write( xI2CPort, ucDataBuffer, 2);
	configASSERT( lReturned == 2 );

	// Wait until the write completes.
	FreeRTOS_ioctl( xI2CPort, ioctlOBTAIN_WRITE_MUTEX, ( void * ) ulMaxDelay );


	for( ulPage = 0UL; ulPage < ( i2cTOTAL_EEPROM_SIZE / i2cPAGE_SIZE ); ulPage++ )
	{
		// Ensure the data buffer does not contain 0x00 already.
		memset( ucDataBuffer, 0xff, sizeof( ucDataBuffer ) );

		// Read a page back from the EEPROM.
		lReturned = FreeRTOS_read( xI2CPort, ucDataBuffer, i2cPAGE_SIZE );
		configASSERT( lReturned == i2cPAGE_SIZE );

		// Check each byte in the page just read contains 0.
		for( ucByte = 0U; ucByte < i2cPAGE_SIZE; ucByte++ )
		{
			configASSERT( ucDataBuffer[ ucByte ] == 0U );
		}
		vPrintStringAndNumber("\n Verified-EEprom-page ",ulPage);

		if(ulPage == 16)
			break;

	}


	/* Do the same, but this time write a different value into each location
	(the value will overflow). */


	ucValue = 0x00U;
	ulStartAddress = 0UL;
	for( ulPage = 0UL; ulPage < ( i2cTOTAL_EEPROM_SIZE / i2cPAGE_SIZE ); ulPage++ )
	{
		for( ucByte = 0U; ucByte < i2cPAGE_SIZE; ucByte++ )
		{
			// ucDataBuffer[ 0 ] & ucDataBuffer[ 1] holds the byte address so is skipped.
			ucDataBuffer[ ucByte + 2 ] = ucValue;
			ucValue++;
		}

		prvWritePageToEEPROM( xI2CPort, ulStartAddress );

		// Move to the next page.
		ulStartAddress += i2cPAGE_SIZE;

		if(ulPage == 16)
			break;

	}


	/* Check all data read from the EEPROM reads as written.  Start by setting
	the	read address back to the start of the EEPROM. */

	ucDataBuffer[ 0 ] = 0x00;
	ucDataBuffer[ 1 ] = 0x00;
	lReturned = FreeRTOS_write( xI2CPort, ucDataBuffer, 2 );
	configASSERT( lReturned == 2 );

	// Wait until the write completes.
	FreeRTOS_ioctl( xI2CPort, ioctlOBTAIN_WRITE_MUTEX, ( void * ) ulMaxDelay );

	ucValue = 0U;
	for( ulPage = 0UL; ulPage < ( i2cTOTAL_EEPROM_SIZE / i2cPAGE_SIZE ); ulPage++ )
	{
		// Ensure the data buffer starts clear.
		memset( ucDataBuffer, 0xff, sizeof( ucDataBuffer ) );

		// Read a page back from the EEPROM.
		lReturned = FreeRTOS_read( xI2CPort, ucDataBuffer, i2cPAGE_SIZE );
		configASSERT( lReturned == i2cPAGE_SIZE );

		// Check each byte in the page contains the expected value.
		for( ucByte = 0U; ucByte < i2cPAGE_SIZE; ucByte++ )
		{
			configASSERT( ucDataBuffer[ ucByte ] == ucValue );
			ucValue++;
		}
		vPrintString("2.EEprom verified\n");

		if(ulPage == 16)
			break;

	}

}
/*-----------------------------------------------------------*/
//
//
//lecture - i2c-eeprom - most significant 2 bits of the byte address
//                       as least 2 bits of the slave address -
//                       balance bits of slave address are set based on  0x50
//          i2c-eeprom - refer to schematic and eeprom hw user manual for device addressing
//                       7-bit addressing scheme is used
//          i2c-eeprom - finally, an ioctl command is used to set the slave address
//                       appropriately
//
static uint8_t prvSetSlaveAddress( Peripheral_Descriptor_t xI2CPort, uint32_t ulByteAddress )
{
uint32_t ulSlaveAddress;

	/* The bottom two bits of the slave address are used as the two most
	significant bits of the byte address within the EEPROM. */
//	ulSlaveAddress = ulByteAddress;
	//ulSlaveAddress >>= 8UL;  //aaa-zzz
	//ulSlaveAddress &= 0x03UL;
//	ulSlaveAddress |= i2cEEPROM_SLAVE_ADDRESS;
    ulSlaveAddress = i2cEEPROM_SLAVE_ADDRESS;  //aaa-zzz
	FreeRTOS_ioctl( xI2CPort, ioctlSET_I2C_SLAVE_ADDRESS, ( void * ) ulSlaveAddress );
    //lecture - i2c-eeprom - 8-bit address of the specific byte in a block is passed to write method
	//          i2c-eeprom - see code below !!!
	return ( uint8_t ) ulByteAddress;
}
/*-----------------------------------------------------------*/
//
//
//lecture - i2c-eeprom - this code is invoked from the above eeprom test code
//          for writing each page !!!
//
//lecture - i2c-eeprom - address passed is in the range - 0x000 - 0x3ff
//          i2c-eeprom - first 2 bits represent the respective block, in the EEPROM
//          i2c-eeprom - balance bits represent the word address, in the respective block
//
//lecture - i2c-eeprom - refer to prvSetSlaveAddress() method for more comments on the
//          slave address
//
//lecture - i2c-eeprom - first byte of the buffer is set to the byte address(least significant)
//          within a given block - the slave address(most significant) contains the
//          device address and block address bits - the combination provides the
//          complete addressing - device address | block address | byte address
//
//
//
//lecture - i2c-eeprom - we need to explore the FreeRTOS+IO code and i2c driver related code
//                       for more details (??)
//
//


static void prvWriteBytesToEEPROM( Peripheral_Descriptor_t xI2CPort, uint32_t ulByteAddress )
{
	int32_t lReturned;
	const uint32_t ulMaxDelay = ( 500UL / portTICK_RATE_MS );

	ucTinyDataBuffer[ 0 ]=0;
	ucTinyDataBuffer[1] = prvSetSlaveAddress( xI2CPort, ulByteAddress );
	lReturned = FreeRTOS_write( xI2CPort, ucTinyDataBuffer, 1 + 2 );
	configASSERT( lReturned == ( 1 + 2 ) );

	FreeRTOS_ioctl( xI2CPort, ioctlOBTAIN_WRITE_MUTEX, ( void * ) ulMaxDelay );
	vTaskDelay( i2cWRITE_CYCLE_DELAY );

	memset( ucTinyDataBuffer, 0x00, sizeof( ucTinyDataBuffer ) );

	ucTinyDataBuffer[ 0 ] = 0x00;
	ucTinyDataBuffer[ 1 ] = 0x00;
	lReturned = FreeRTOS_write( xI2CPort, ucTinyDataBuffer,2);
	configASSERT( lReturned == 2 );

	FreeRTOS_ioctl( xI2CPort, ioctlOBTAIN_WRITE_MUTEX, ( void * ) ulMaxDelay );
	lReturned = FreeRTOS_read( xI2CPort, ucTinyDataBuffer, 1);
	configASSERT( lReturned == 1);

	vPrintString("\nverified");
	//vPrintStringAndNumber("\nucTinyDataBuffer %d %d",ucTinyDataBuffer[1],ucTinyDataBuffer[2] );
	vPrintStringAndNumber("\nucTinyDataBuffer",ucTinyDataBuffer[0] );


}


static void prvWritePageToEEPROM( Peripheral_Descriptor_t xI2CPort, uint32_t ulByteAddress )
{
int32_t lReturned;
const uint32_t ulMaxDelay = ( 500UL / portTICK_RATE_MS );


    ucDataBuffer[0]= ulByteAddress / 256 ;

    ucDataBuffer[1]= ulByteAddress % 256 ;


	/* The first byte of the transmitted data contains the least significant
	byte of the EEPROM byte address.  The slave address holds the most
	significant bits of the EEPROM byte address. */
	//ucDataBuffer[ 0 ] = prvSetSlaveAddress( xI2CPort, ulByteAddress );
    prvSetSlaveAddress( xI2CPort, ulByteAddress );

    //
	//
	//lecture - i2c-eeprom - eventually, we invoke FreeRTOS_write() method - refer
	//                       to FreeRTOS_DriverInterface.h - invoked using a macro
	//
	//
	//
	/* Write the buffer to the EEPROM. */
	lReturned = FreeRTOS_write( xI2CPort, ucDataBuffer, i2cPAGE_SIZE + 2 );
	configASSERT( lReturned == ( i2cPAGE_SIZE + 2 ) );
    //
	//
	//lecture - i2c-eeprom - mutex operation will be effective only for
	//                       for zero copy tx mode - otherwise, will be ignored
	//
	//
	/* Wait until the write in complete, then allow it to program.  This is a
	very crude way of doing it - the EEPROM could alternatively be polled. */
	FreeRTOS_ioctl( xI2CPort, ioctlOBTAIN_WRITE_MUTEX, ( void * ) ulMaxDelay );
	//
	//lecture - i2c-eeprom - here, we block the task for a certain time -
	//                       so that,further operations i2c operations can be
	//                       initiated - this is done as per eeprom hw manual
	//
	//
	vTaskDelay( i2cWRITE_CYCLE_DELAY );
}

