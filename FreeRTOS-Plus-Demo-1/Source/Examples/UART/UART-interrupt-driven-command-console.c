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

/* Standard includes. */
#include "string.h"
//
//lecture - headers of FreeRTOS - need to explore nested headers and add comments, as needed
//
/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
//
//lecture - header of FreeRTOS+IO  -
//          need to explore nested headers and add comments, as needed
//
/* FreeRTOS+IO includes. */
#include "FreeRTOS_IO.h"

//
//
//lecture - headers related to FreeRTOS+CLI and example - command console
//
//
/* Example includes. */
#include "FreeRTOS_CLI.h"
#include "UART-interrupt-driven-command-console.h"

//
//lecture - we need to check, where buffers are used and how they are allocated ??
//

/* Dimensions the buffer into which input characters are placed. */
#define cmdMAX_INPUT_SIZE			50

/* Place holder for calls to ioctl that don't use the value parameter. */
#define cmdPARAMTER_NOT_USED		( ( void * ) 0 )

//lecture - we need to check, where the time-outs are used ??
//
/* Block times of 50 and 500milliseconds, specified in ticks. */
#define cmd50ms						( ( void * ) ( 50UL / portTICK_RATE_MS ) )
#define cmd500ms					( ( void * ) ( 500UL / portTICK_RATE_MS ) )
/*-----------------------------------------------------------*/

/*
 * The task that implements the command console processing.
 */
static void prvUARTCommandConsoleTask( void *pvParameters );

/*-----------------------------------------------------------*/
//
//lecture - we are creating a task to handle command console, using UART ???
//
/* Holds the handle of the task that implements the UART command console. */
static xTaskHandle xCommandConsoleTask = NULL;

static const int8_t * const pcWelcomeMessage = ( int8_t * ) "FreeRTOS command server.\r\nType Help to view a list of registered commands.\r\n\r\n>";
static const int8_t * const pcNewLine = ( int8_t * ) "\r\n";
static const int8_t * const pcEndOfCommandOutputString = ( int8_t * ) "\r\n[Press ENTER to execute the previous command again]\r\n>";
//
//lecture - active peripheral handle of the UART, used by command console / task
//
/* The peripheral used by the command interpreter (and the vWriteString()
function). */
static Peripheral_Descriptor_t xConsoleUART = NULL;

/*-----------------------------------------------------------*/
//
//
//lecture - UART- a command console task is created - it will be handling the command console job
//
//
void vUARTCommandConsoleStart( void )
{
	xTaskCreate( 	prvUARTCommandConsoleTask,				/* The task that implements the command console. */
					( const int8_t * const ) "UARTCmd",		/* Text name assigned to the task.  This is just to assist debugging.  The kernel does not use this name itself. */
					configUART_COMMAND_CONSOLE_STACK_SIZE,	/* The size of the stack allocated to the task. */
					NULL,									/* The parameter is not used, so NULL is passed. */
					configUART_COMMAND_CONSOLE_TASK_PRIORITY,/* The priority allocated to the task. */
					&xCommandConsoleTask );					/* Used to store the handle to the created task. */
}
/*-----------------------------------------------------------*/
//
//
//
//lecture - many features are from FREERTOS+CLI subsystem - explore related headers and
//          source files as needed ???
//
//lecture - several calls are to FREERTOS+IO subsystems - explore related headers and
//          source files as needed ???
//
//
//
//
//
static void prvUARTCommandConsoleTask( void *pvParameters )
{
int8_t cRxedChar, cInputIndex = 0, *pcOutputString;
static int8_t cInputString[ cmdMAX_INPUT_SIZE ], cLastInputString[ cmdMAX_INPUT_SIZE ];
portBASE_TYPE xReturned;

	( void ) pvParameters;

	/* Obtain the address of the output buffer.  Note there is no mutual
	exclusion on this buffer as it is assumed only one command console
	interface will be used at any one time. */
	pcOutputString = FreeRTOS_CLIGetOutputBuffer();

	/* Open the UART port used for console input.  The second parameter
	(ulFlags) is not used in this case.  The default board rate is set by the
	boardDEFAULT_UART_BAUD parameter.  The baud rate can be changed using a
	FreeRTOS_ioctl() call with the ioctlSET_SPEED command. */
	//lecture - UART - refer to FreeRTOS_DriverInterface.c for FreeRTOS_open() api
	//        - UART - refer to LPCXpresso17xx-base-board.h for
	//                 boardCOMMAND_CONSOLE_UART - it is UART3 of the four UARTs
	//
	//
	//lecture - UART - we will receive NULL, if there is a failure
	//
	//
	xConsoleUART = FreeRTOS_open( boardCOMMAND_CONSOLE_UART, ( uint32_t ) cmdPARAMTER_NOT_USED );
	configASSERT( xConsoleUART );

	//
	//lecture - UART - refer to FreeRTOS_DriverInterface.c for FreeRTOS_ioctl() api
	//
	//
	/* Change the Tx usage model from straight polled mode to use zero copy
	buffers with interrupts.  In this mode, the UART will transmit characters
	directly from the buffer passed to the FreeRTOS_write()	function. */
	xReturned = FreeRTOS_ioctl( xConsoleUART, ioctlUSE_ZERO_COPY_TX, cmdPARAMTER_NOT_USED );
	configASSERT( xReturned );
    //
	//
	//
	//lecture -UART - refer to FreeRTOS_DriverInterface.c for FreeRTOS_ioctl() api
	//
    //
	/* Change the Rx usage model from straight polled mode to use a character
	queue.  Character queue reception is appropriate in this case as characters
	can only be received as quickly as they can be typed, and need to be parsed
	character by character. */
	xReturned = FreeRTOS_ioctl( xConsoleUART, ioctlUSE_CHARACTER_QUEUE_RX, ( void * ) cmdMAX_INPUT_SIZE );
	configASSERT( xReturned );

	//
	//lecture - UART - refer to refer to FreeRTOS_DriverInterface.c for FreeRTOS_ioctl() api -
	//                 eventually, invokes the uart specific ioctl method and respective setting
	//
	//lecture - UART - may set different priorities and check ???
	//
	/* By default, the UART interrupt priority will have been set to the lowest
	possible.  It must be kept at or below configMAX_LIBRARY_INTERRUPT_PRIORITY,
	but	can be raised above its default priority using a FreeRTOS_ioctl() call
	with the ioctlSET_INTERRUPT_PRIORITY command. */
	xReturned = FreeRTOS_ioctl( xConsoleUART, ioctlSET_INTERRUPT_PRIORITY, ( void * ) ( configMIN_LIBRARY_INTERRUPT_PRIORITY - 1 ) );
	configASSERT( xReturned );
    //
	//
	//
	//lecture - UART - as per zero copy tx mode, we must first acquire the mutex, before we can
	//                 initiate a write on the device instance - in this case, it is uart
	//
	//lecture - UART - refer to FreeRTOS_DriverInterface.c for code and details of FreeRTOS_ioctl()
	//
	//
	//
	/* Send the welcome message. */
	if( FreeRTOS_ioctl( xConsoleUART, ioctlOBTAIN_WRITE_MUTEX, cmd50ms ) == pdPASS )
	{
		FreeRTOS_write( xConsoleUART, pcWelcomeMessage, strlen( ( char * ) pcWelcomeMessage ) );
	}

	for( ;; )
	{

		FreeRTOS_read( xConsoleUART, &cRxedChar, sizeof( cRxedChar ) );

		/* Echo the character back. */
		if( FreeRTOS_ioctl( xConsoleUART, ioctlOBTAIN_WRITE_MUTEX, cmd50ms ) == pdPASS )
		{
			FreeRTOS_write( xConsoleUART, &cRxedChar, sizeof( cRxedChar ) );
		}

		if( cRxedChar == '\n' )
		{
			/* The input command string is complete.  Ensure the previous
			UART transmission has finished before sending any more data.
			This task will be held in the Blocked state while the Tx completes,
			if it has not already done so, so no CPU time will be wasted by
			polling. */
			if( FreeRTOS_ioctl( xConsoleUART, ioctlOBTAIN_WRITE_MUTEX, cmd50ms ) == pdPASS )
			{
				/* Start to transmit a line separator, just to make the output
				easier to read. */
				FreeRTOS_write( xConsoleUART, pcNewLine, strlen( ( char * ) pcNewLine ) );
			}

			/* See if the command is empty, indicating that the last command is
			to be executed again. */
			if( cInputIndex == 0 )
			{
				strcpy( ( char * ) cInputString, ( char * ) cLastInputString );
			}

			/* Pass the received command to the command interpreter.  The
			command interpreter is called repeatedly until it returns
			pdFALSE as it might generate more than one string. */
			do
			{
				/* Once again, just check to ensure the UART has completed
				sending whatever it was sending last.  This task will be held
				in the Blocked state while the Tx completes, if it has not
				already done so, so no CPU time	is wasted polling. */
				xReturned = FreeRTOS_ioctl( xConsoleUART, ioctlOBTAIN_WRITE_MUTEX, cmd50ms );
				if( xReturned == pdPASS )
				{
					/* Get the string to write to the UART from the command
					interpreter. */
					xReturned = FreeRTOS_CLIProcessCommand( cInputString, pcOutputString, configCOMMAND_INT_MAX_OUTPUT_SIZE );

					/* Write the generated string to the UART. */
					FreeRTOS_write( xConsoleUART, pcOutputString, strlen( ( char * ) pcOutputString ) );
				}

			} while( xReturned != pdFALSE );

			/* All the strings generated by the input command have been sent.
			Clear the input	string ready to receive the next command.  Remember
			the command that was just processed first in case it is to be
			processed again. */
			strcpy( ( char * ) cLastInputString, ( char * ) cInputString );
			cInputIndex = 0;
			memset( cInputString, 0x00, cmdMAX_INPUT_SIZE );

			/* Ensure the last string to be transmitted has completed. */
			if( FreeRTOS_ioctl( xConsoleUART, ioctlOBTAIN_WRITE_MUTEX, cmd50ms ) == pdPASS )
			{
				/* Start to transmit a line separator, just to make the output
				easier to read. */
				FreeRTOS_write( xConsoleUART, pcEndOfCommandOutputString, strlen( ( char * ) pcEndOfCommandOutputString ) );
			}
		}
		else
		{
			if( cRxedChar == '\r' )
			{
				/* Ignore the character. */
			}
			else if( cRxedChar == '\b' )
			{
				/* Backspace was pressed.  Erase the last character in the
				string - if any. */
				if( cInputIndex > 0 )
				{
					cInputIndex--;
					cInputString[ cInputIndex ] = '\0';
				}
			}
			else
			{
				/* A character was entered.  Add it to the string
				entered so far.  When a \n is entered the complete
				string will be passed to the command interpreter. */
				if( ( cRxedChar >= ' ' ) && ( cRxedChar <= '~' ) )
				{
					if( cInputIndex < cmdMAX_INPUT_SIZE )
					{
						cInputString[ cInputIndex ] = cRxedChar;
						cInputIndex++;
					}
				}
			}
		}
	}
}
/*-----------------------------------------------------------*/

void vOutputString( const uint8_t * const pucMessage )
{
	/* Obtaining the write mutex prevents strings output using this function
	from corrupting strings output by the command interpreter task (and visa
	versa).  It does not, however, prevent complete strings output using this
	function intermingling with complete strings output from the command
	interpreter as the command interpreter only holds the mutex on an output
	string by output string basis. */
	if( xConsoleUART != NULL )
	{
		if( FreeRTOS_ioctl( xConsoleUART, ioctlOBTAIN_WRITE_MUTEX, cmd500ms ) == pdPASS )
		{
			FreeRTOS_write( xConsoleUART, pucMessage, strlen( ( char * ) pucMessage ) );
		}
	}
}
/*-----------------------------------------------------------*/


