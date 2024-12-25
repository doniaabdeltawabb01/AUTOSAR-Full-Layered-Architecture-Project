/*************************************************************************
 *
 * Module: Port
 *
 * File Name: Port_Cfg.h
 *
 * Description: Pre-Compile Configuration Header file for TM4C123GH6PM Microcontroller - Port Driver.
 *
 * Author: Donia Abdeltawab
 ***************************************************************************/
#ifndef PORT_CFG_H
#define PORT_CFG_H


/*
 * Module Version 1.0.0
 */

#define PORT_CFG_SW_MAJOR_VERSION              (1U)
#define PORT_CFG_SW_MINOR_VERSION              (0U)
#define PORT_CFG_SW_PATCH_VERSION              (0U)

/*
 * AUTOSAR Version 4.0.3
 */
#define PORT_CFG_AR_RELEASE_MAJOR_VERSION     (4U)
#define PORT_CFG_AR_RELEASE_MINOR_VERSION     (0U)
#define PORT_CFG_AR_RELEASE_PATCH_VERSION     (3U)



/* Pre-compile option for Development Error Detect */
#define PORT_DEV_ERROR_DETECT                (STD_ON)

/* Pre-compile option for Version Info API */
#define PORT_VERSION_INFO_API                (STD_ON)

/* Precompile option for setting Pin direction API */
#define PORT_SET_PIN_DIRECTION_API            (STD_ON)
											  
/* Precompile option to set pin mode API */
#define PORT_SET_PIN_MODE_API                 (STD_ON)

 
/* Precompile and Post Build option for Port pin mode changeability */
#define PORT_PIN_MODE_CHANGEABLE              (STD_ON)   			


/* Pre-compile parameter for number of pins */
#define Port_Pins_Number                  0x27   //39 pin  


//Pins modes
#define Port_Pin_Mode_ADC        (0U)
#define Port_Pin_Mode_DIO        (1U)
#define Port_Pin_Mode_UART1      (2U) 
#define Port_Pin_Mode_UART2      (3U) 
#define Port_Pin_Mode_SSI1       (4U)
#define Port_Pin_Mode_SSI2       (5U)
#define Port_Pin_Mode_I2C        (6U)
#define Port_Pin_Mode_M0PWM      (7U)
#define Port_Pin_Mode_CAN        (8U)
#define Port_Pin_Mode_NMI        (9U)
#define Port_Pin_Mode_USB        (10U)
#define Port_Pin_Mode_QEI        (11U) 
#define Port_Pin_Mode_GPT        (12U)
#define Port_Pin_Mode_ANALOG_COMP  (13U)
#define Port_Pin_Mode_CORE         (14U)




//port pin id number
#define PORT_A_PIN_0    (0U)
#define PORT_A_PIN_1    (1U)
#define PORT_A_PIN_2    (2U)
#define PORT_A_PIN_3    (3U)
#define PORT_A_PIN_4    (4U)
#define PORT_A_PIN_5    (5U)
#define PORT_A_PIN_6    (6U)
#define PORT_A_PIN_7    (7U)
   
#define PORT_B_PIN_0    (8U)
#define PORT_B_PIN_1    (9U)
#define PORT_B_PIN_2    (10U) 
#define PORT_B_PIN_3    (11U) 
#define PORT_B_PIN_4    (12U) 
#define PORT_B_PIN_5    (13U) 
#define PORT_B_PIN_6    (14U)
#define PORT_B_PIN_7    (15U)
   
#define PORT_C_PIN_0    (16U)
#define PORT_C_PIN_1    (17U) 
#define PORT_C_PIN_2    (18U)
#define PORT_C_PIN_3    (19U)
#define PORT_C_PIN_4    (20U)
#define PORT_C_PIN_5    (21U)
#define PORT_C_PIN_6    (22U) 
#define PORT_C_PIN_7    (23U)
   
#define PORT_D_PIN_0    (24U)
#define PORT_D_PIN_1    (25U)   
#define PORT_D_PIN_2    (26U)   
#define PORT_D_PIN_3    (27U)   
#define PORT_D_PIN_4    (28U)   
#define PORT_D_PIN_5    (29U)   
#define PORT_D_PIN_6    (30U)   
#define PORT_D_PIN_7    (31U)

#define PORT_E_PIN_0    (32U)
#define PORT_E_PIN_1    (33U)
#define PORT_E_PIN_2    (34U)
#define PORT_E_PIN_3    (35U)
#define PORT_E_PIN_4    (36U)
#define PORT_E_PIN_5    (37U)
   
#define PORT_F_PIN_0    (38U)
#define PORT_F_PIN_1    (39U)
#define PORT_F_PIN_2    (40U)
#define PORT_F_PIN_3    (41U)
#define PORT_F_PIN_4    (42U)


//port_num
#define PortA     (0U)
#define PortB     (1U)
#define PortC     (2U)  
#define PortD     (3U)
#define PortE     (4U)
#define PortF     (5U)

//pin_num
#define Pin_num0 (0U)
#define Pin_num1 (1U)
#define Pin_num2 (2U)
#define Pin_num3 (3U)
#define Pin_num4 (4U)
#define Pin_num5 (5U)
#define Pin_num6 (6U)
#define Pin_num7 (7U)


//pin mode changeability

//PORTA modes status

#define PortA_Pin0_DirStatus    STD_OFF
#define PortA_Pin1_DirStatus    STD_OFF
#define PortA_Pin2_DirStatus    STD_OFF
#define PortA_Pin3_DirStatus    STD_OFF
#define PortA_Pin4_DirStatus    STD_OFF
#define PortA_Pin5_DirStatus    STD_OFF
#define PortA_Pin6_DirStatus    STD_OFF
#define PortA_Pin7_DirStatus    STD_OFF


//PORTB modes status
#define PortB_Pin0_DirStatus    STD_OFF
#define PortB_Pin1_DirStatus    STD_OFF
#define PortB_Pin2_DirStatus    STD_OFF
#define PortB_Pin3_DirStatus    STD_OFF
#define PortB_Pin4_DirStatus    STD_OFF
#define PortB_Pin5_DirStatus    STD_OFF
#define PortB_Pin6_DirStatus    STD_OFF
#define PortB_Pin7_DirStatus    STD_OFF


//PORTC modes status
#define PortC_Pin0_DirStatus    STD_OFF
#define PortC_Pin1_DirStatus    STD_OFF
#define PortC_Pin2_DirStatus    STD_OFF
#define PortC_Pin3_DirStatus    STD_OFF
#define PortC_Pin4_DirStatus    STD_OFF
#define PortC_Pin5_DirStatus    STD_OFF
#define PortC_Pin6_DirStatus    STD_OFF
#define PortC_Pin7_DirStatus    STD_OFF


//PORTD modes status
#define PortD_Pin0_DirStatus    STD_OFF
#define PortD_Pin1_DirStatus    STD_OFF
#define PortD_Pin2_DirStatus    STD_OFF
#define PortD_Pin3_DirStatus    STD_OFF
#define PortD_Pin4_DirStatus    STD_OFF
#define PortD_Pin5_DirStatus    STD_OFF
#define PortD_Pin6_DirStatus    STD_OFF
#define PortD_Pin7_DirStatus    STD_OFF




//PORTE modes status
#define PortE_Pin0_DirStatus    STD_OFF
#define PortE_Pin1_DirStatus    STD_OFF
#define PortE_Pin2_DirStatus    STD_OFF
#define PortE_Pin3_DirStatus    STD_OFF
#define PortE_Pin4_DirStatus    STD_OFF
#define PortE_Pin5_DirStatus    STD_OFF



//PORTF modes status
#define PortF_Pin0_DirStatus    STD_OFF
#define PortF_Pin1_DirStatus    STD_OFF
#define PortF_Pin2_DirStatus    STD_OFF
#define PortF_Pin3_DirStatus    STD_OFF
#define PortF_Pin4_DirStatus    STD_OFF







#endif
