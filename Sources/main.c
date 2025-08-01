/* ###################################################################
**     Filename    : main.c
**     Project     : LabFinalProject
**     Processor   : MK64FN1M0VLL12
**     Version     : Driver 01.01
**     Compiler    : GNU C Compiler
**     Date/Time   : 2019-11-03, 17:50, # CodeGen: 0
**     Abstract    :
**         Main module.
**         This module contains user's application code.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
 ** @file main.c
 ** @version 01.01
 ** @brief
 **         Main module.
 **         This module contains user's application code.
 */
/*!
 **  @addtogroup main_module main module documentation
 **  @{
 */
/* MODULE main */


/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "Pins1.h"
#include "FX1.h"
#include "GI2C1.h"
#include "WAIT1.h"
#include "CI2C1.h"
#include "CsIO1.h"
#include "IO1.h"
#include "MCUC1.h"
#include "SM1.h"
#include "MK64F12.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "PDD_Includes.h"
#include "Init_Config.h"

/* User includes (#include below this line is not maintained by Processor Expert) */

/*lint -save  -e970 Disable MISRA rule (6.3) checking. */

unsigned char write[512];
void delay_seconds(uint32_t seconds) {
    uint32_t startTime = FTM3_CNT; // Capture the starting counter value
    uint32_t ticksPerSecond = (48000000 / 128); // Calculate ticks per second (prescaler = 128) (gotta check the system clock)

    while (((FTM3_CNT - startTime) & 0xFFFF) < (seconds * ticksPerSecond)) {
    }
}
void startMotor(uint32_t delayLong){
	uint32_t i =0;
	GPIOD_PDOR = 0x36;
	for(i=0; i < delayLong; i++);
	GPIOD_PDOR = 0x35;
	for(i=0; i < delayLong; i++);
	GPIOD_PDOR = 0x39;
	for(i=0; i < delayLong; i++);
	GPIOD_PDOR = 0x3A;
	for(i=0; i < delayLong; i++);
}
int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
{
	/* Write your local variable definition here */

	/*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
	PE_low_level_init();
	/*** End of Processor Expert internal initialization.                    ***/
	/* Write your code here */
//***************************************************************************************************************************** */
	//Setting up ADC0
	SIM_SCGC6 |= SIM_SCGC6_ADC0_MASK; // 0x8000000u; Enable ADC0 Clock
	uint32_t photoresist1 =0;
	uint32_t photoresist2 =0;
	uint32_t photoresist3 =0;
	ADC0_CFG1 = 0x0C; // 16bits ADC; Bus Clock
	ADC0_SC1A = 0x1F; // Disable the module, ADCH = 11111
//***************************************************************************************************************************** */
    //setting up stepper motor and inputs
	SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK; /* Enable Port A Clock Gate Control */
    SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK; /* Enable Port B Clock Gate Control */
    SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK; /* Enable Port D Clock Gate Control */
    /* Configure PC[8:7, 5:0], PD[7:0] for GPIO*/
    PORTB_GPCLR = 0x00FF0100;  // Configure Port B
    PORTD_GPCLR = 0x00FF0100; /*Configure Port D Pins 0-7 for GPIO;*/
    PORTA_GPCLR = 0x00FF0100;
    /* Configure PC[8:7, 5:0], PD[7:0] for output mode */
    GPIOB_PDDR = 0x00000000; // setip as input instead ***FIXME***
    GPIOD_PDDR = 0x000000FF; /*Configure Port D Pins 0-7 for Output;*/
    GPIOA_PDDR = 0x000000FF; //Configures Port A pins 0-7 as output;
    uint32_t i = 0;
    uint32_t mode1 = 0;
    uint32_t mode2 = 0;
    uint32_t type1 = 0;
    uint32_t type2 = 0;
    uint32_t input = 0;
    uint32_t delayLong = 10000; //1000000;
    GPIOD_PDOR = 0; //Setting D to 0
    GPIOA_PDOR = 0;
    uint32_t portionType = 0;
    uint32_t timeEat = 0;
    uint32_t thresholdLight = 0;

//***************************************************************************************************************************** */
   //Sets up timer
    SIM_SCGC3 |= SIM_SCGC3_FTM3_MASK; // FTM3 clock gating on
    FTM3_MODE = 0x05; // Enable FTM3
    FTM3_MOD = 0xFFFF; // Max counter value
    // Set prescale factor and clock source (FTMx_SC[4:0])
    FTM3_SC = 0x0F; // 0x0F => b01111 => system clock / 128
    uint32_t timer_val = FTM3_CNT; // Get current counter value
    uint32_t activeDelay = 0; //false

    //***************************************************************************************************************************** */
    //Setting up UART

	//printf("Hello\n");

	for(;;) {
//***************************************************************************************************************************** */
//setting up the stepper motor
		input = GPIOB_PDIR;
		mode1 = input & 0x4; // Port B pin 2 (1)
		mode2 = input & 0x8; // port B pin 3 (2)
		type1 = input & 0x400;// Port B Pin 10 (3)
		type2 = input & 0x800; // PortB pin 11 (4)
		//Test what photoresistor shows what
		//Photoresistor1
		ADC0_SC1A = 0x00; //Write to SC1A to start conversion from ADC_DP0
		while(ADC0_SC2 & ADC_SC2_ADACT_MASK); // Conversion in progress
		while(!(ADC0_SC1A & ADC_SC1_COCO_MASK)); // Until conversion complete
		photoresist1 =ADC0_RA;
		printf("PHOTO1");
		printf("%d",photoresist1);
		for(uint32_t delay = 0; delay < 300000; delay++);
		//printf("\n");
		ADC0_SC1A = 0x1F;
		//Photoresistor2
		ADC0_SC1A = 0x01; //Write to SC1A to start conversion from ADC_DP1
		while(ADC0_SC2 & ADC_SC2_ADACT_MASK); // Conversion in progress
		while(!(ADC0_SC1A & ADC_SC1_COCO_MASK)); // Until conversion complete
		photoresist2 =ADC0_RA;
		printf(" PHOTO2");
		printf("%d",photoresist2);
		//printf("\n");
		ADC0_SC1A = 0x1F;
		for(uint32_t delay = 0; delay < 300000; delay++);
		//Photoresitor3
		ADC0_SC1A = 0x02; //Write to SC1A to start conversion from ADC_DP2
		while(ADC0_SC2 & ADC_SC2_ADACT_MASK); // Conversion in progress
		while(!(ADC0_SC1A & ADC_SC1_COCO_MASK)); // Until conversion complete
		photoresist3 =ADC0_RA;
		printf(" PHOTO3");
		printf("%d",photoresist3);
		printf("\n");
		ADC0_SC1A = 0x1F;
		for(uint32_t delay = 0; delay < 300000; delay++); //delay



		//DIP SWITCH
		//0 0 setting portions
		if((mode1 == 0) && (mode2 == 0)){
			if((type1 == 0) && (type2 !=0)){ // dipSwitch 3 and 4 (0 1)
				portionType = 1;
				//setup led1 to be on
				//leds 2 and 3 off
				GPIOA_PDOR |= (1UL<<0);
				GPIOA_PDOR &= ~(1UL <<1);
				GPIOA_PDOR &= ~(1UL <<2);
			}
			else if((type1 != 0) && (type2 ==0)){// dipSwitch 3 and 4 (1 0)
				portionType = 2;
				//setup led2 to be on
				//leds 1 and 3 off
				GPIOA_PDOR |= (1UL<<1);
				GPIOA_PDOR &= ~(1UL <<0);
				GPIOA_PDOR &= ~(1UL <<2);
			}
			else if((type1 != 0) && (type2 !=0)){ // dipSwitch 3 and 4 (1 1)
				portionType = 3;
				//setup led3 to be on
				//leds 1 and 2 off
				GPIOA_PDOR |= (1UL<<2);
				GPIOA_PDOR &= ~(1UL <<0);
				GPIOA_PDOR &= ~(1UL <<1);
			}
			else{
				//all leds off
				GPIOA_PDOR &= ~(1UL<<0);
				GPIOA_PDOR &= ~(1UL <<1);
				GPIOA_PDOR &= ~(1UL <<2);
			}
		}
		// 0 1 Set timer mode
		else if((mode1 == 0) && (mode2 != 0)){
			if((type1 == 0) && (type2 !=0)){ // 0 1 for dipSwitch 3,4
				timeEat = 30; // 30 seconds but fs 30 minutes
				//led 4 on
				//led 5 and 6 off
			}
			else if((type1 != 0) && (type2 ==0)){ // 1 0 for dipSwitch 3,4
				timeEat = 60;
				//led 5 on
				//led 4 and 6 off
			}
			else if((type1 != 0) && (type2 !=0)){ // 1 1 for dipSwitch 3,4
				timeEat = 90;
				//led 6 on
				//led 4 and 5 off
			}
		}
		//1 0 AutoMode Begin
		else if((mode1 != 0) && (mode2 == 0)){
			//timer setting up
			printf("Portion");
			printf("%d",portionType);
			printf("\n");
			printf("Delay");
			printf("%d",timeEat);
			printf("\n");
			if(portionType!= 0){
				delay_seconds(timeEat);
					//if(delay_seconds(timeEat,activeDelay)){
					if(portionType == 1){
					// check photoresistor to see if its full, if not start motor
						ADC0_SC1A = 0x00; //Write to SC1A to start conversion from ADC_DP0
						while(ADC0_SC2 & ADC_SC2_ADACT_MASK); // Conversion in progress
						while(!(ADC0_SC1A & ADC_SC1_COCO_MASK)); // Until conversion complete
						photoresist1 =ADC0_RA;
						if(photoresist1 < thresholdLight){
							startMotor(delayLong);
						}
					}
					else if(portionType == 2){
					// check photoresistor to see if its full, if not start motor
						ADC0_SC1A = 0x01; //Write to SC1A to start conversion from ADC_DP1
						while(ADC0_SC2 & ADC_SC2_ADACT_MASK); // Conversion in progress
						while(!(ADC0_SC1A & ADC_SC1_COCO_MASK)); // Until conversion complete
						photoresist2 =ADC0_RA;
						if(photoresist2 < thresholdLight){
							startMotor(delayLong);
						}
					}
					else if(portionType == 3){
					// check photoresistor to see if its full, if not start motor
						ADC1_SC1A = 0x01; // Write to SC1A to start conversion from ADC1_DP1
						while (ADC1_SC2 & ADC_SC2_ADACT_MASK); // Wait while the conversion is in progress
						while (!(ADC1_SC1A & ADC_SC1_COCO_MASK)); // Wait until conversion is complete
						photoresist3 = ADC1_RA; // Read the conversion result
						if(photoresist3 < thresholdLight){
							startMotor(delayLong);
						}
				}
			}

		}
		//1 1 TURN ON MOTOR
		else if((mode1 != 0) && (mode2 != 0)){
			GPIOD_PDOR = 0x36;
			for(i=0; i < delayLong; i++);
			GPIOD_PDOR = 0x35;
			for(i=0; i < delayLong; i++);
			GPIOD_PDOR = 0x39;
			for(i=0; i < delayLong; i++);
			GPIOD_PDOR = 0x3A;
			for(i=0; i < delayLong; i++);
		}

//***************************************************************************************************************************** */


	}


	/* For example: for(;;) { } */

	/*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */
/*!
 ** @}
 */
/*
 ** ###################################################################
 **
 **     This file was created by Processor Expert 10.4 [05.11]
 **     for the Freescale Kinetis series of microcontrollers.
 **
 ** ###################################################################
 */
