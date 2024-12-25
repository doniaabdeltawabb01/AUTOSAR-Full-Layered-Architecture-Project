 /******************************************************************************
 *
 * Module: Port
 *
 * File Name: Port.h
 *
 * Description: Header file for TM4C123GH6PM Microcontroller - Port Driver.
 *
 * Author: Mohamed Tarek
 ******************************************************************************/

#ifndef PORT_H
#define PORT_H

#include "Common_Macros.h"
#include "Std_Types.h"
#include "Port_Cfg.h"
#include "tm4c123gh6pm_registers.h"

#define PORT_VENDOR_ID_H  (2000U)

/* Port Module Id */
#define PORT_MODULE_ID    (124U)

/* PORT Instance Id */
#define PORT_INSTANCE_ID  (0U)


/*
 * Module Version 1.0.0
 */
#define PORT_SW_MAJOR_VERSION           (1U)
#define PORT_SW_MINOR_VERSION           (0U)
#define PORT_SW_PATCH_VERSION           (0U)

/*
 * AUTOSAR Version 4.0.3
 */
#define PORT_AR_RELEASE_MAJOR_VERSION   (4U)
#define PORT_AR_RELEASE_MINOR_VERSION   (0U)
#define PORT_AR_RELEASE_PATCH_VERSION   (3U)

/*
 * Macros for PORT Status
 */
#define PORT_INITIALIZED                (1U)
#define PORT_NOT_INITIALIZED            (0U)


/* AUTOSAR checking between Std Types and PORT Modules */
#if ((STD_TYPES_AR_RELEASE_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)\
 ||  (STD_TYPES_AR_RELEASE_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION)\
 ||  (STD_TYPES_AR_RELEASE_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
  #error "The AR version of Std_Types.h does not match the expected version"
#endif

/* AUTOSAR Version checking between PORT_Cfg.h and PORT.h files */

#if ((PORT_CFG_AR_RELEASE_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)\
 ||  (PORT_CFG_AR_RELEASE_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION)\
 ||  (PORT_CFG_AR_RELEASE_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
  #error "The AR version of Port_Cfg.h does not match the expected version"
#endif

   /* Software Version checking between PORT_Cfg.h and PORT.h files */
#if ((PORT_CFG_SW_MAJOR_VERSION != PORT_SW_MAJOR_VERSION)\
 ||  (PORT_CFG_SW_MINOR_VERSION != PORT_SW_MINOR_VERSION)\
 ||  (PORT_CFG_SW_PATCH_VERSION != PORT_SW_PATCH_VERSION))
  #error "The SW version of Port_Cfg.h does not match the expected version"
#endif   
   
 /*******************************************************************************************
                                 API Service Id 
*********************************************************************************************/

/* Service IDs */

  /* API service ID for PORT Init function.
   */
#define PORT_INIT_ID                    (uint8)0x00

/*   API service ID for PORT set pin direction function.
 */
#define PORT_SETPINDIRECTION_ID         (uint8)0x01

/*  API service ID for PORT refresh pin direction function.
 */
#define PORT_REFRESHPINDIRECTION_ID     (uint8)0x02

/*
 API service ID for PORT get version info function.
 */
#define PORT_GETVERSIONINFO_ID          (uint8)0x03


#define PORT_SETPINMODE_ID              (uint8)0x04   
/***********************************************************************/

/*******************************************************************************
 *                      DET Error Codes                                        *
 *******************************************************************************/
/* Invalid Port Pin ID requested */
#define PORT_E_PARAM_PIN                (uint8)0x0A

/* Port Pin not configured as changeable */
#define PORT_E_DIRECTION_UNCHANGEABLE   (uint8)0x0B
 
/* API Port_Init service called with wrong parameter */
#define PORT_E_PARAM_CONFIG             (uint8)0x0C

/* API Port_SetPinMode service called with uncomabtible mode for selected pin */
#define PORT_E_PARAM_INVALID_MODE       (uint8)0x0D

/* API Port_SetPinMode service called when mode is unchangeable. */
#define PORT_E_MODE_UNCHANGEABLE        (uint8)0x0E

/* API service called without module initialization */
#define PORT_E_UNINIT                   (uint8)0x0F

/* APIs called with a Null Pointer */
#define PORT_E_PARAM_POINTER            (uint8)0x10




/*******************************************************************************
 *                              Module Data Types                              *
 *******************************************************************************/

/* Description: Enum to hold PIN direction */
typedef enum
{
    INPUT,OUTPUT
}Port_PinDirection;

typedef uint8 Port_PinModeType; 

typedef uint8 Port_PinType;
/* Description: Enum to hold internal resistor type for PIN */
typedef enum
{
    OFF,PULL_UP,PULL_DOWN
}Port_InternalResistor;

/* Description: Structure to configure each individual PIN:
 *	1. the PORT Which the pin belongs to. 0, 1, 2, 3, 4 or 5
 *	2. the number of the pin in the PORT.
 *      3. the direction of pin --> INPUT or OUTPUT
 *      4. the internal resistor --> Disable, Pull up or Pull down
 */
typedef struct 
{
    uint8 port_num; 
    uint8 pin_num; 
    Port_PinDirection direction;
    Port_InternalResistor resistor;
    uint8 initial_value;   //stdhigh or stdlow
    uint8 pin_mode; 
    uint8 pin_direction_changable;

}Port_ConfigPins;

typedef struct
{
	Port_ConfigPins Pins[Port_Pins_Number];   
}Port_ConfigType;

extern  Port_ConfigType Port_Configuration;

/*******************************************************************************
 *                      Function Prototypes                                    *
 *******************************************************************************/

/************************************************************************************
* Service Name: Port_Init
* Sync/Async: Synchronous
* Reentrancy: Non-reentrant
* Parameters (in): ConfigPtr - Pointer to post-build configuration data
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Function to Initializes the port driver module 
*              -initialize ALL ports and port pins with the configuration set pointed to by the parameter ConfigPtr
*              - Setup the pin as Digital GPIO pin
*              - Setup the direction of the GPIO pin
*              - Setup the internal resistor for i/p pin

************************************************************************************/
void Port_Init(const Port_ConfigType* ConfigPtr);


/****************************************
* Service Name: Port_SetPinDirection 
* Sync/Async: Synchronous
* Reentrancy: reentrant
* Parameters (in): Pin, Port Pin ID number, direction, Port Pin Direction 
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Function Sets the port pin direction
              
**************************************/
void Port_SetPinDirection(Port_PinType Pin, Port_PinDirection Direction);



/****************************************
* Service Name: Port_RefreshPortDirection  
* Sync/Async: Synchronous
* Reentrancy: Non-reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Function Refreshes port direction
              
**************************************/

void Port_RefreshPortDirection(void);

/****************************************
* Service Name: Port_GetVersionInfo 
* Sync/Async: Synchronous
* Reentrancy: Non-reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out):  Pointer to where to store the version information of this module. 
* Return value: None
* Description: Function Returns the version information of this module. 
              
**************************************/


void Port_GetVersionInfo(Std_VersionInfoType* versioninfo); 


/****************************************
* Service Name: Port_SetPinMode  
* Sync/Async: Synchronous
* Reentrancy:reentrant
* Parameters (in): Pin ,Port Pin ID number , Mode, New Prt Pin mode to be set on port pin
* Parameters (inout): None
* Parameters (out):  None
* Return value: None
* Description: Function Sets the port pin mode. 
              
**************************************/
void Port_SetPinMode( Port_PinType Pin,Port_PinModeType Mode); 



#endif /* PORT_H */
