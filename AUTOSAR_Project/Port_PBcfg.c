
/*************************************************************************
 *
 * Module: Port
 *
 * File Name: Port_PBCfg.h
 *
 * Description: Pre-Compile Configuration Header file for TM4C123GH6PM Microcontroller - Port Driver.
 *
 * Author: Donia Abdeltawab
 ***************************************************************************/


#ifndef PORT_PBCFG_C_
#define PORT_PBCFG_C_

#include "Port.h"


/*
* SW Module Version is 1.0.0
*/
#define PORT_PBCFG_SW_MAJOR_VERSION             (1U)
#define PORT_PBCFG_SW_MINOR_VERSION             (0U)
#define PORT_PBCFG_SW_PATCH_VERSION             (0U)

/*
* AUTOSAR Version is 4.0.3 compatible
*/
#define PORT_PBCFG_AR_RELEASE_MAJOR_VERSION     (4U)
#define PORT_PBCFG_AR_RELEASE_MINOR_VERSION     (0U)
#define PORT_PBCFG_AR_RELEASE_PATCH_VERSION     (3U)

/* Check compatibility of Port.h AUTOSAR version with
* Port_PBcfg.c AUTOSAR version.
*/
#if ((PORT_PBCFG_AR_RELEASE_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)\
 ||  (PORT_PBCFG_AR_RELEASE_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION)\
 ||  (PORT_PBCFG_AR_RELEASE_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
	#error "The AR version of Port.h does not match the expected version"
#endif

/* Check compatibility of Port.h SW version with
* Port_PBcfg.c SW version.
*/
#if ((PORT_PBCFG_SW_MAJOR_VERSION != PORT_SW_MAJOR_VERSION)\
 ||  (PORT_PBCFG_SW_MINOR_VERSION != PORT_SW_MINOR_VERSION)\
 ||  (PORT_PBCFG_SW_PATCH_VERSION != PORT_SW_PATCH_VERSION))
	#error "The AR version of Port.h does not match the expected version"
#endif


 Port_ConfigType Port_Configuration= 
{
PortA,Pin_num0,INPUT,PULL_UP,STD_LOW,Port_Pin_Mode_DIO,PortA_Pin0_DirStatus,
PortA,Pin_num1,INPUT,PULL_UP,STD_LOW,Port_Pin_Mode_DIO,PortA_Pin1_DirStatus,
PortA,Pin_num2,INPUT,PULL_UP,STD_LOW,Port_Pin_Mode_DIO,PortA_Pin2_DirStatus,
PortA,Pin_num3,INPUT,PULL_UP,STD_LOW,Port_Pin_Mode_DIO,PortA_Pin3_DirStatus,
PortA,Pin_num4,INPUT,PULL_UP,STD_LOW,Port_Pin_Mode_DIO,PortA_Pin4_DirStatus,
PortA,Pin_num5,INPUT,PULL_UP,STD_LOW,Port_Pin_Mode_DIO,PortA_Pin5_DirStatus,
PortA,Pin_num6,INPUT,PULL_UP,STD_LOW,Port_Pin_Mode_DIO,PortA_Pin6_DirStatus,
PortA,Pin_num7,INPUT,PULL_UP,STD_LOW,Port_Pin_Mode_DIO,PortA_Pin7_DirStatus,

PortB,Pin_num0,INPUT,PULL_UP,STD_LOW,Port_Pin_Mode_DIO,PortB_Pin0_DirStatus,
PortB,Pin_num1,INPUT,PULL_UP,STD_LOW,Port_Pin_Mode_DIO,PortB_Pin1_DirStatus,
PortB,Pin_num2,INPUT,PULL_UP,STD_LOW,Port_Pin_Mode_DIO,PortB_Pin2_DirStatus,
PortB,Pin_num3,INPUT,PULL_UP,STD_LOW,Port_Pin_Mode_DIO,PortB_Pin3_DirStatus,
PortB,Pin_num4,INPUT,PULL_UP,STD_LOW,Port_Pin_Mode_DIO,PortB_Pin4_DirStatus,
PortB,Pin_num5,INPUT,PULL_UP,STD_LOW,Port_Pin_Mode_DIO,PortB_Pin5_DirStatus,
PortB,Pin_num6,INPUT,PULL_UP,STD_LOW,Port_Pin_Mode_DIO,PortB_Pin6_DirStatus,
PortB,Pin_num7,INPUT,PULL_UP,STD_LOW,Port_Pin_Mode_DIO,PortB_Pin7_DirStatus,


//PortC,Pin_num0,INPUT,PULL_UP,STD_LOW,Port_Pin_Mode_DIO,PortC_Pin0_DirStatus,
//PortC,Pin_num1,INPUT,PULL_UP,STD_LOW,Port_Pin_Mode_DIO,PortC_Pin1_DirStatus,
//PortC,Pin_num2,INPUT,PULL_UP,STD_LOW,Port_Pin_Mode_DIO,PortC_Pin2_DirStatus,
//PortC,Pin_num3,INPUT,PULL_UP,STD_LOW,Port_Pin_Mode_DIO,PortC_Pin3_DirStatus,
PortC,Pin_num4,INPUT,PULL_UP,STD_LOW,Port_Pin_Mode_DIO,PortC_Pin4_DirStatus,
PortC,Pin_num5,INPUT,PULL_UP,STD_LOW,Port_Pin_Mode_DIO,PortC_Pin5_DirStatus,
PortC,Pin_num6,INPUT,PULL_UP,STD_LOW,Port_Pin_Mode_DIO,PortC_Pin6_DirStatus,
PortC,Pin_num7,INPUT,PULL_UP,STD_LOW,Port_Pin_Mode_DIO,PortC_Pin7_DirStatus,

PortD,Pin_num0,INPUT,PULL_UP,STD_LOW,Port_Pin_Mode_DIO,PortD_Pin0_DirStatus,
PortD,Pin_num1,INPUT,PULL_UP,STD_LOW,Port_Pin_Mode_DIO,PortD_Pin1_DirStatus,
PortD,Pin_num2,INPUT,PULL_UP,STD_LOW,Port_Pin_Mode_DIO,PortD_Pin2_DirStatus,
PortD,Pin_num3,INPUT,PULL_UP,STD_LOW,Port_Pin_Mode_DIO,PortD_Pin3_DirStatus,
PortD,Pin_num4,INPUT,PULL_UP,STD_LOW,Port_Pin_Mode_DIO,PortD_Pin4_DirStatus,
PortD,Pin_num5,INPUT,PULL_UP,STD_LOW,Port_Pin_Mode_DIO,PortD_Pin5_DirStatus,
PortD,Pin_num6,INPUT,PULL_UP,STD_LOW,Port_Pin_Mode_DIO,PortD_Pin6_DirStatus,
PortD,Pin_num7,INPUT,PULL_UP,STD_LOW,Port_Pin_Mode_DIO,PortD_Pin7_DirStatus,

PortE,Pin_num0,INPUT,PULL_UP,STD_LOW,Port_Pin_Mode_DIO,PortE_Pin0_DirStatus,
PortE,Pin_num1,INPUT,PULL_UP,STD_LOW,Port_Pin_Mode_DIO,PortE_Pin1_DirStatus,
PortE,Pin_num2,INPUT,PULL_UP,STD_LOW,Port_Pin_Mode_DIO,PortE_Pin2_DirStatus,
PortE,Pin_num3,INPUT,PULL_UP,STD_LOW,Port_Pin_Mode_DIO,PortE_Pin3_DirStatus,
PortE,Pin_num4,INPUT,PULL_UP,STD_LOW,Port_Pin_Mode_DIO,PortE_Pin4_DirStatus,
PortE,Pin_num5,INPUT,PULL_UP,STD_LOW,Port_Pin_Mode_DIO,PortE_Pin5_DirStatus,


PortF,Pin_num0,INPUT,PULL_UP,STD_LOW,Port_Pin_Mode_DIO,PortF_Pin0_DirStatus,
PortF,Pin_num1,OUTPUT,OFF,STD_HIGH,Port_Pin_Mode_DIO,PortF_Pin1_DirStatus,
PortF,Pin_num2,OUTPUT,PULL_DOWN,STD_LOW,Port_Pin_Mode_DIO,PortF_Pin2_DirStatus,
PortF,Pin_num3,OUTPUT,PULL_DOWN,STD_LOW,Port_Pin_Mode_DIO,PortF_Pin3_DirStatus,
PortF,Pin_num4,INPUT,PULL_DOWN,STD_LOW,Port_Pin_Mode_DIO,PortF_Pin4_DirStatus,



};






#endif
