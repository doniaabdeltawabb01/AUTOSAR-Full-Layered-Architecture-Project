 /******************************************************************************
 *
 * Module: Port
 *
 * File Name: Port.c
 *
 * Description: Source file for TM4C123GH6PM Microcontroller - Port Driver.
 *
 * Author: Mohamed Tarek
 ******************************************************************************/

#include "Port.h"
#include "Port_Regs.h"


#if (PORT_DEV_ERROR_DETECT == STD_ON)

#include "Det.h"
/* AUTOSAR Version checking between Det and Dio Modules */
#if ((DET_AR_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)\
 || (DET_AR_MINOR_VERSION  != PORT_AR_RELEASE_MINOR_VERSION)\
 || (DET_AR_PATCH_VERSION  != PORT_AR_RELEASE_PATCH_VERSION))
  #error "The AR version of Det.h does not match the expected version"
#endif

#endif

STATIC const Port_ConfigPins * PortConfig = NULL_PTR;
STATIC uint8 Port_Status = PORT_NOT_INITIALIZED;
/************************************************************************************
* Service Name: Port_Init
* Sync/Async: Synchronous
* Service ID[hex]:0x00
* Reentrancy: Non-reentrant
* Parameters (in): ConfigPtr - Pointer to post-build configuration data
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Function to Initializes the Port Driver module
*       
************************************************************************************/
void Port_Init(const Port_ConfigType* ConfigPtr)
{

#if (PORT_DEV_ERROR_DETECT == STD_ON)
	/* check if the input configuration pointer is not a NULL_PTR */
	if (NULL_PTR == ConfigPtr)
	{
		Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_INIT_ID,
		     PORT_E_PARAM_CONFIG);
	}
	else
#endif
	{
		/*
		 * Set the module state to initialized and point to the PB configuration structure using a global pointer.
		 * This global pointer is global to be used by other functions to read the PB configuration structures
		 */
		Port_Status = PORT_INITIALIZED;
		PortConfig = ConfigPtr->Pins; /* address of the first pins structure --> Pins[0] */
	volatile uint32 * PortGpio_Ptr = NULL_PTR; /* point to the required Port Registers base address */
     volatile uint32 delay = 0;
     uint8 Pins;
     for(Pins=0; Pins<Port_Pins_Number ;Pins++)
     {
       switch(PortConfig[Pins].port_num)
       {
           case  0: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS; /* PORTA Base Address */
      	         break;
        	case  1: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS; /* PORTB Base Address */
      	         break;
          case  2: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS; /* PORTC Base Address */
      	         break;
          case  3: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS; /* PORTD Base Address */
      	         break;
           case  4: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS; /* PORTE Base Address */
      	         break;
           case  5: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS; /* PORTF Base Address */
      	         break;
       }
       //accessing the clock
        SYSCTL_REGCGC2_REG |= (1<<PortConfig[Pins].port_num);
        delay = SYSCTL_REGCGC2_REG;
        //pins need unlocking
        if( ((PortConfig[Pins].port_num == 3) && (PortConfig[Pins].pin_num == 7)) || ((PortConfig[Pins].port_num == 5) && (PortConfig[Pins].pin_num == 0)) ) /* PD7 or PF0 */
         {
             *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_LOCK_REG_OFFSET) = 0x4C4F434B;                     /* Unlock the GPIOCR register */   
             SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_COMMIT_REG_OFFSET) , PortConfig[Pins].pin_num);  /* Set the corresponding bit in GPIOCR register to allow changes on this pin */
         }
         else if( (PortConfig[Pins].port_num == 2) && (PortConfig[Pins].pin_num <= 3) ) /* PC0 to PC3 */
         {
            continue; /* Do Nothing ...  this is the JTAG pins */
         }
         else
         {
             /* Do Nothing ... No need to unlock the commit register for this pin */
          }  



        //direction
        if(PortConfig[Pins].direction == OUTPUT)
        {
           SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET) , PortConfig[Pins].pin_num);               /* Set the corresponding bit in the GPIODIR register to configure it as output pin */
            //initial value
            if(PortConfig[Pins].initial_value == STD_HIGH)
            {
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DATA_REG_OFFSET) , PortConfig[Pins].pin_num);          /* Set the corresponding bit in the GPIODATA register to provide initial value 1 */
            }
            else
            {
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DATA_REG_OFFSET) , PortConfig[Pins].pin_num);        /* Clear the corresponding bit in the GPIODATA register to provide initial value 0 */
            }
        }
        else if(PortConfig[Pins].direction == INPUT)
        {
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET) , PortConfig[Pins].pin_num);             /* Clear the corresponding bit in the GPIODIR register to configure it as input pin */
            
            //pull up/pull down

            if(PortConfig[Pins].resistor == PULL_UP)
            {
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_UP_REG_OFFSET) , PortConfig[Pins].pin_num);       /* Set the corresponding bit in the GPIOPUR register to enable the internal pull up pin */
            }
            else if(PortConfig[Pins].resistor == PULL_DOWN)
            {
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_DOWN_REG_OFFSET) , PortConfig[Pins].pin_num);     /* Set the corresponding bit in the GPIOPDR register to enable the internal pull down pin */
            }
            else
            {
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_UP_REG_OFFSET) , PortConfig[Pins].pin_num);     /* Clear the corresponding bit in the GPIOPUR register to disable the internal pull up pin */
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_DOWN_REG_OFFSET) , PortConfig[Pins].pin_num);   /* Clear the corresponding bit in the GPIOPDR register to disable the internal pull down pin */
            }
        }
        else
        {
            /* Do Nothing */
        }

        /////mode
        if(PortConfig[Pins].pin_mode==Port_Pin_Mode_ADC)
        {
               SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , PortConfig[Pins].pin_num);
               SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , PortConfig[Pins].pin_num);
               CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , PortConfig[Pins].pin_num); 
               CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET) , PortConfig[Pins].pin_num);
               *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (PortConfig[Pins].pin_num * 4));  
        }


        else if(PortConfig[Pins].pin_mode==Port_Pin_Mode_DIO)
        {
              CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) ,  PortConfig[Pins].pin_num);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
              CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) ,  PortConfig[Pins].pin_num);             /* Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register */
              *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << ( PortConfig[Pins].pin_num* 4));     /* Clear the PMCx bits for this pin */
              SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) ,  PortConfig[Pins].pin_num);         /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
        } 


        else if(PortConfig[Pins].pin_mode==Port_Pin_Mode_UART1)
        {
               SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , PortConfig[Pins].pin_num);
               CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , PortConfig[Pins].pin_num);
               SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , PortConfig[Pins].pin_num); 
               *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000001 << (PortConfig[Pins].pin_num * 4)); 
        }


        else if(PortConfig[Pins].pin_mode==Port_Pin_Mode_UART2)
        {
              if(Pins==20 || Pins==21 )
              {
               SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , PortConfig[Pins].pin_num);
               CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , PortConfig[Pins].pin_num);
               SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , PortConfig[Pins].pin_num); 
               *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000002 << (PortConfig[Pins].pin_num * 4)); 
              }      
              else
              {
                Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_INIT_ID,PORT_E_PARAM_CONFIG);
              }      
        }


        else if(PortConfig[Pins].pin_mode==Port_Pin_Mode_SSI1)
         {
            if(Pins==24 || Pins==25 || Pins==26 || Pins==27 )
              {
               SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , PortConfig[Pins].pin_num);
               CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , PortConfig[Pins].pin_num);
               SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , PortConfig[Pins].pin_num); 
               *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000001 << (PortConfig[Pins].pin_num * 4));  
              }
              else
              {
                Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_INIT_ID,PORT_E_PARAM_CONFIG);
              }
         }


        else if(PortConfig[Pins].pin_mode==Port_Pin_Mode_SSI2)
         {
               SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , PortConfig[Pins].pin_num);
               CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , PortConfig[Pins].pin_num);
               SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , PortConfig[Pins].pin_num); 
               *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000002 << (PortConfig[Pins].pin_num * 4));  
         }


        else if(PortConfig[Pins].pin_mode==Port_Pin_Mode_I2C)
        {
               SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , PortConfig[Pins].pin_num);
               CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , PortConfig[Pins].pin_num);
               SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , PortConfig[Pins].pin_num); 
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_OPEN_DRAIN) , PortConfig[Pins].pin_num); 
               *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000003 << (PortConfig[Pins].pin_num * 4));  
        }


        else if(PortConfig[Pins].pin_mode==Port_Pin_Mode_M0PWM)
        {     
               SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , PortConfig[Pins].pin_num);
               CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , PortConfig[Pins].pin_num);
               SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , PortConfig[Pins].pin_num);
            if(Pins==12 || Pins==13 ||Pins==14 || Pins==15 || Pins==16 || Pins==17 || Pins==18 || Pins==19 || Pins==20 || Pins==21 || Pins==24 || Pins==25 || Pins==36 || Pins==37 )
              {
 
               *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000004 << (PortConfig[Pins].pin_num * 4));  
              }   
              else
              {
                *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000005 << (PortConfig[Pins].pin_num * 4));  
              }     
        }
      
 
        else if(PortConfig[Pins].pin_mode==Port_Pin_Mode_CAN )
        {
               SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , PortConfig[Pins].pin_num);
               CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , PortConfig[Pins].pin_num);
               SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , PortConfig[Pins].pin_num);
               if(Pins==38 || Pins==41)
               {
                 *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000003 << (PortConfig[Pins].pin_num * 4));  
               }  
               else
               {
                 *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000008 << (PortConfig[Pins].pin_num * 4));  
               }
        }

        else if(PortConfig[Pins].pin_mode==Port_Pin_Mode_NMI )
        {
               SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , PortConfig[Pins].pin_num);
               CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , PortConfig[Pins].pin_num);
               SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , PortConfig[Pins].pin_num);
               *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000008 << (PortConfig[Pins].pin_num * 4));  
       }
 
        else if(PortConfig[Pins].pin_mode==Port_Pin_Mode_USB )
        {
               SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , PortConfig[Pins].pin_num);
               CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , PortConfig[Pins].pin_num);
               SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , PortConfig[Pins].pin_num);
               *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000008 << (PortConfig[Pins].pin_num * 4));    
        }

        else if(PortConfig[Pins].pin_mode==Port_Pin_Mode_QEI )
        {
              SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , PortConfig[Pins].pin_num);
               CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , PortConfig[Pins].pin_num);
               SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , PortConfig[Pins].pin_num);
               *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000006 << (PortConfig[Pins].pin_num * 4));               
        }

        else if(PortConfig[Pins].pin_mode==Port_Pin_Mode_GPT ) //TXCCP , WTXCCP
        {
               SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , PortConfig[Pins].pin_num);
               CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , PortConfig[Pins].pin_num);
               SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , PortConfig[Pins].pin_num);
               *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000007 << (PortConfig[Pins].pin_num * 4));               
        }
        

         else if(PortConfig[Pins].pin_mode==Port_Pin_Mode_ANALOG_COMP ) //C0
        {
            if(Pins==20 || Pins==21 || Pins==22 || Pins==23) //c1-,c1+,c0+,c0-
            {
               SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , PortConfig[Pins].pin_num);
               SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , PortConfig[Pins].pin_num);
               CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , PortConfig[Pins].pin_num);
               *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &=~ (0x0000000f << (PortConfig[Pins].pin_num * 4));               
            }
            else
            {
               SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , PortConfig[Pins].pin_num);
               CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , PortConfig[Pins].pin_num);
               SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , PortConfig[Pins].pin_num);
               *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000009 << (PortConfig[Pins].pin_num * 4));                
            }
        }       

        else if(PortConfig[Pins].pin_mode==Port_Pin_Mode_CORE  ) //trd0,trd1,trcllk
        {
               SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , PortConfig[Pins].pin_num);
               CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , PortConfig[Pins].pin_num);
               SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , PortConfig[Pins].pin_num);
               *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x0000000E << (PortConfig[Pins].pin_num * 4));               
        }

     }

    }
}


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
#if(PORT_SET_PIN_DIRECTION_API==STD_ON)
void Port_SetPinDirection(Port_PinType Pin, Port_PinDirection Direction)
{

  volatile uint32 * PortGpio_Ptr = NULL_PTR;
	boolean error = FALSE;
  #if (PORT_DEV_ERROR_DETECT == STD_ON)
	/* Check if the Driver is initialized before using this function */
   if (Port_Status ==PORT_NOT_INITIALIZED )
   {
   	Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID,PORT_INIT_ID, PORT_E_UNINIT);
   	error = TRUE;
   }
   else
   {
   	/* No Action Required */
   }
   if (Pin >= Port_Pins_Number )
   {
   	Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_INIT_ID,PORT_E_PARAM_PIN);
    error = TRUE;
   } 
   else
   {

   }
        
       if(PortConfig[Pin].pin_direction_changable == STD_OFF)
        {
          Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_INIT_ID,
  	PORT_E_DIRECTION_UNCHANGEABLE);
          error = TRUE;
        }
        else 
        {
        }
#endif

   if(error== FALSE)
   {
Port_Status =PORT_INITIALIZED;
       switch(PortConfig[Pin].port_num)
       {
         case  0: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS; /* PORTA Base Address */
      	         break;
         case  1: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS; /* PORTB Base Address */
      	         break;
          case  2: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS; /* PORTC Base Address */
      	         break;
          case  3: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS; /* PORTD Base Address */
      	         break;
           case  4: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS; /* PORTE Base Address */
      	         break;
           case  5: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS; /* PORTF Base Address */
      	         break;
       }
       if(Direction==INPUT)
       {
        CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET) , PortConfig[Pin].pin_num);
       }
       else
       {
        SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET) , PortConfig[Pin].pin_num);
       }

    }

}
#endif

    
 

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

void Port_RefreshPortDirection(void)
{
   #if (PORT_DEV_ERROR_DETECT == STD_ON)
	/* Check if the Driver is initialized before using this function */
   if (Port_Status ==PORT_NOT_INITIALIZED )
   {
   	Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID,PORT_INIT_ID, PORT_E_UNINIT);
   }
   else
   {
   	/* No Action Required */
   }
   #endif 
    volatile uint32 * PortGpio_Ptr = NULL_PTR; /* point to the required Port Registers base address */
    uint8 Pins;
    for(Pins=0;Pins<Port_Pins_Number;Pins++)
    {
       if(PortConfig[Pins].pin_direction_changable  ==STD_OFF)
       {
       switch(PortConfig[Pins].port_num)
       {
         case  0: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS; /* PORTA Base Address */
      	         break;
         case  1: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS; /* PORTB Base Address */
      	         break;
          case  2: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS; /* PORTC Base Address */
      	         break;
          case  3: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS; /* PORTD Base Address */
      	         break;
           case  4: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS; /* PORTE Base Address */
      	         break;
           case  5: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS; /* PORTF Base Address */
      	         break;
       }
         if(BIT_IS_SET(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET) , PortConfig[Pins].pin_num))
            {
             SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET) , PortConfig[Pins].pin_num); 
            }
           else 
           {
             if(BIT_IS_CLEAR(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET) , PortConfig[Pins].pin_num))
            {
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET) , PortConfig[Pins].pin_num); 
            }
            else
             {/* No Action Required */}
           }
 
        }
        else
        {
          continue;
        }

    
    

    }



}

    


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

#if (PORT_VERSION_INFO_API == STD_ON)
	void Port_GetVersionInfo( Std_VersionInfoType* versioninfo )
	{
	
	#if (PORT_DEV_ERROR_DETECT == STD_ON)
		/* Check if the pointer is not a Null pointer */
		if(NULL_PTR == versioninfo)
		{
			/* Report to DET  */
			Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID,PORT_GETVERSIONINFO_ID ,PORT_E_PARAM_POINTER);
		}
		else
	#endif 
		{
			
			versioninfo->vendorID =(uint16)PORT_VENDOR_ID_H;
			
			versioninfo->moduleID =(uint16)PORT_MODULE_ID;
			
			versioninfo->sw_major_version = (uint8)PORT_SW_MAJOR_VERSION;
			
			versioninfo->sw_minor_version = (uint8)PORT_SW_MINOR_VERSION;
			
			versioninfo->sw_patch_version = (uint8)PORT_SW_PATCH_VERSION;
		}
	}
#endif


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
#if ( PORT_SET_PIN_MODE_API==STD_ON)
void Port_SetPinMode( Port_PinType Pin, Port_PinModeType Mode)
{
 if (Port_Status == PORT_NOT_INITIALIZED)
	{
		Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_INIT_ID,PORT_E_UNINIT);
                
	}
	else
	{

	}
 
  /* Incorrect Port Pin ID passed*/
  if (Pin >= Port_Pins_Number )
	{
		Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_INIT_ID,PORT_E_PARAM_PIN);
	}
  else
  {
  }
  /* Incorrect Port Pin Mode passed*/
  if ((Mode < Port_Pin_Mode_ADC) || (Mode > Port_Pin_Mode_CORE ))
	{
		
		Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SETPINMODE_ID , PORT_E_PARAM_INVALID_MODE);
  }
  else
  {
  }
  
    /* check if Port Pin mode not configured as changeable */
	if( PORT_PIN_MODE_CHANGEABLE==STD_OFF)
  {
    Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SETPINMODE_ID , PORT_E_MODE_UNCHANGEABLE);
  }
  else
  {

  }
 volatile uint32 * PortGpio_Ptr = NULL_PTR; /* point to the required Port Registers base address */

       switch(PortConfig[Pin].port_num)
       {
         case  0: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS; /* PORTA Base Address */
      	         break;
         case  1: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS; /* PORTB Base Address */
      	         break;
          case  2: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS; /* PORTC Base Address */
      	         break;
          case  3: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS; /* PORTD Base Address */
      	         break;
           case  4: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS; /* PORTE Base Address */
      	         break;
           case  5: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS; /* PORTF Base Address */
      	         break;
       }
 if(Mode==Port_Pin_Mode_ADC)
        {
               SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , PortConfig[Pin].pin_num);
               SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , PortConfig[Pin].pin_num);
               CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , PortConfig[Pin].pin_num); 
               CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET) , PortConfig[Pin].pin_num);
               *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (PortConfig[Pin].pin_num * 4));  
        }


        else if(Mode==Port_Pin_Mode_DIO)
        {
              CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) ,  PortConfig[Pin].pin_num);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
              CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) ,  PortConfig[Pin].pin_num);             /* Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register */
              *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << ( PortConfig[Pin].pin_num* 4));     /* Clear the PMCx bits for this pin */
              SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) ,  PortConfig[Pin].pin_num);         /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
        } 


        else if(Mode==Port_Pin_Mode_UART1)
        {
               SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , PortConfig[Pin].pin_num);
               CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , PortConfig[Pin].pin_num);
               SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , PortConfig[Pin].pin_num); 
               *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000001 << (PortConfig[Pin].pin_num * 4)); 
        }


        else if(Mode==Port_Pin_Mode_UART2)
        {
              if(Pin==20 || Pin==21 )
              {
               SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , PortConfig[Pin].pin_num);
               CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , PortConfig[Pin].pin_num);
               SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , PortConfig[Pin].pin_num); 
               *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000002 << (PortConfig[Pin].pin_num * 4)); 
              }      
              else
              {
                Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_INIT_ID,PORT_E_PARAM_CONFIG);
              }      
        }


        else if(Mode==Port_Pin_Mode_SSI1)
         {
            if(Pin==24 || Pin==25 || Pin==26 || Pin==27 )
              {
               SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , PortConfig[Pin].pin_num);
               CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , PortConfig[Pin].pin_num);
               SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , PortConfig[Pin].pin_num); 
               *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000001 << (PortConfig[Pin].pin_num * 4));  
              }
              else
              {
                Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_INIT_ID,PORT_E_PARAM_CONFIG);
              }
         }


        else if(Mode==Port_Pin_Mode_SSI2)
         {
               SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , PortConfig[Pin].pin_num);
               CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , PortConfig[Pin].pin_num);
               SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , PortConfig[Pin].pin_num); 
               *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000002 << (PortConfig[Pin].pin_num * 4));  
         }


        else if(Mode==Port_Pin_Mode_I2C)
        {
               SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , PortConfig[Pin].pin_num);
               CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , PortConfig[Pin].pin_num);
               SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , PortConfig[Pin].pin_num); 
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_OPEN_DRAIN) , PortConfig[Pin].pin_num); 
               *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000003 << (PortConfig[Pin].pin_num * 4));  
        }


        else if(Mode==Port_Pin_Mode_M0PWM)
        {     
               SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , PortConfig[Pin].pin_num);
               CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , PortConfig[Pin].pin_num);
               SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , PortConfig[Pin].pin_num);
            if(Pin==12 || Pin==13 ||Pin==14 || Pin==15 || Pin==16 || Pin==17 || Pin==18 || Pin==19 || Pin==20 || Pin==21 || Pin==24 || Pin==25 || Pin==36 || Pin==37 )
              {
 
               *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000004 << (PortConfig[Pin].pin_num * 4));  
              }   
              else
              {
                *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000005 << (PortConfig[Pin].pin_num * 4));  
              }     
        }
      
 
        else if(Mode==Port_Pin_Mode_CAN )
        {
               SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , PortConfig[Pin].pin_num);
               CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , PortConfig[Pin].pin_num);
               SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , PortConfig[Pin].pin_num);
               if(Pin==38 || Pin==41)
               {
                 *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000003 << (PortConfig[Pin].pin_num * 4));  
               }  
               else
               {
                 *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000008 << (PortConfig[Pin].pin_num * 4));  
               }
        }

        else if(Mode==Port_Pin_Mode_NMI )
        {
               SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , PortConfig[Pin].pin_num);
               CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , PortConfig[Pin].pin_num);
               SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , PortConfig[Pin].pin_num);
               *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000008 << (PortConfig[Pin].pin_num * 4));  
       }
 
        else if(Mode==Port_Pin_Mode_USB )
        {
               SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , PortConfig[Pin].pin_num);
               CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , PortConfig[Pin].pin_num);
               SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , PortConfig[Pin].pin_num);
               *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000008 << (PortConfig[Pin].pin_num * 4));    
        }

        else if(Mode==Port_Pin_Mode_QEI )
        {
              SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , PortConfig[Pin].pin_num);
               CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , PortConfig[Pin].pin_num);
               SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , PortConfig[Pin].pin_num);
               *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000006 << (PortConfig[Pin].pin_num * 4));               
        }

        else if(Mode==Port_Pin_Mode_GPT ) //TXCCP , WTXCCP
        {
               SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , PortConfig[Pin].pin_num);
               CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , PortConfig[Pin].pin_num);
               SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , PortConfig[Pin].pin_num);
               *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000007 << (PortConfig[Pin].pin_num * 4));               
        }
        

         else if(Mode==Port_Pin_Mode_ANALOG_COMP ) //C0
        {
            if(Pin==20 || Pin==21 || Pin==22 || Pin==23) //c1-,c1+,c0+,c0-
            {
               SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , PortConfig[Pin].pin_num);
               SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , PortConfig[Pin].pin_num);
               CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , PortConfig[Pin].pin_num);
               *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &=~ (0x0000000f << (PortConfig[Pin].pin_num * 4));               
            }
            else
            {
               SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , PortConfig[Pin].pin_num);
               CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , PortConfig[Pin].pin_num);
               SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , PortConfig[Pin].pin_num);
               *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000009 << (PortConfig[Pin].pin_num * 4));                
            }
        }       

        else if(Mode==Port_Pin_Mode_CORE  ) //trd0,trd1,trcllk
        {
               SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , PortConfig[Pin].pin_num);
               CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , PortConfig[Pin].pin_num);
               SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , PortConfig[Pin].pin_num);
               *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x0000000E << (PortConfig[Pin].pin_num * 4));               
        }



       
    

}
#endif
