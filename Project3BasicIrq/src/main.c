#include <MKL25Z4.H>
#include "../include/gpio_defs.h"
#include "../include/settings.h"
#include <stdbool.h>
#include <stdint.h>

int oscState = OSC_OFF;      // Oscilloscope State
uint32_t seed = 0x12345678 ; // Random seed
int redLedOn = 0 ;           // This is the state
int redCounter = 0 ;         // This count times the flashes
int greenLedOn = 0 ;         // This is the state
int greenCounter = 0 ;       // This count times the flashes
// Button signal
//   Set in the interupt handler
//   Cleared by the task in the main cycle
volatile unsigned buttonSignal=0;
// Ticks counter
volatile unsigned SysTickCounter ;

int buttonEvent = BUTTON_NOT_PRESSED;
int systemState = NOT_FLASHING;

/* ------------------------------------------
       ECS642U Lab3
   Interrupts in cyclic systems
	 
	 -------------------------------------------- */
	
/*----------------------------------------------------------------------------
  Delay function
 *----------------------------------------------------------------------------*/
void Delay(unsigned int time_del) {
	// delay is about 1 millisecond * time_del
	volatile int t;
	
	while (time_del--) {
		for (t=4800; t>0; t--)
			;
	}
}

/*----------------------------------------------------------------------------
  GPIO Configuration

  Configure the port B pins for the on-board red and green LEDs as an output
    Red LED connected to Port B (PTB), bit 18 (RED_LED_POS)
    Green LED connected to Port B (PTB), bit 19 (GREEN_LED_POS)

 *----------------------------------------------------------------------------*/
void configureGPIOoutput() {
		// Configuration steps
	//   1. Enable clock to GPIO ports
	//   2. Enable GPIO ports
	//   3. Set GPIO direction to output
	//   4. Ensure LEDs are off

	// Enable clock to ports B 
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK ;
	
	// Make the pins GPIO
	PORTB->PCR[RED_LED_POS] &= ~PORT_PCR_MUX_MASK;          
	PORTB->PCR[RED_LED_POS] |= PORT_PCR_MUX(1);          
	PORTB->PCR[GREEN_LED_POS] &= ~PORT_PCR_MUX_MASK;          
	PORTB->PCR[GREEN_LED_POS] |= PORT_PCR_MUX(1);          
	PORTB->PCR[OSCIL_POS] &= ~PORT_PCR_MUX_MASK;          
	PORTB->PCR[OSCIL_POS] |= PORT_PCR_MUX(1);          
	
	
	// Set ports to outputs
	PTB->PDDR |= MASK(RED_LED_POS) ;
	PTB->PDDR |= MASK(GREEN_LED_POS) ;
	PTB->PDDR |= MASK(OSCIL_POS) ;

	// Turn off the LED
	PTB->PSOR = MASK(RED_LED_POS) ;
	PTB->PSOR = MASK(GREEN_LED_POS) ;
	PTB->PSOR = MASK(OSCIL_POS) ;
}

/*----------------------------------------------------------------------------
  redOn: Set red LED on

  Note use of the clear register (c.f. the data output)
 *----------------------------------------------------------------------------*/
void redOn(void) {
	   // set red on without changing anything else
	   // LED is actve low
		 PTB->PCOR |= MASK(RED_LED_POS) ;
}

/*----------------------------------------------------------------------------
  redOff: Set red LED off

  Note use of the clear register (c.f. the data output)
 *----------------------------------------------------------------------------*/
void redOff(void) {
	   // set red off with changing anything else
	   // LED is actve low
		 PTB->PSOR |= MASK(RED_LED_POS) ;
}

/*----------------------------------------------------------------------------
  greenOn: Set green LED on

  Note use of the clear register (c.f. the data output)
 *----------------------------------------------------------------------------*/
void greenOn(void) {
	   // set red on without changing anything else
	   // LED is actve low
		 PTB->PCOR |= MASK(GREEN_LED_POS) ;
}

/*----------------------------------------------------------------------------
  greenOff: Set green LED off

  Note use of the clear register (c.f. the data output)
 *----------------------------------------------------------------------------*/
void greenOff(void) {
	   // set red off with changing anything else
	   // LED is actve low
		 PTB->PSOR |= MASK(GREEN_LED_POS) ;
}

void oscToogle(void) {
	if (oscState == OSC_ON)
	{
		PTB->PSOR |= MASK(OSCIL_POS) ;
	}
	else
	{		
		PTB->PCOR |= MASK(OSCIL_POS) ;
	}
	oscState = !oscState;
}

bool isPressed(void) {
	if (PTB->PDIR & MASK(BUTTON_POS)) {
			return false ;
	}
	
	return true ;
}
/*----------------------------------------------------------------------------
  GPIO Input Configuration

  Initialse a Port D pin as an input, with
    - a pullup resistor 
		- an interrupt on the falling edge
  Bit number given by BUTTON_POS - bit 6, corresponding to J2, pin 14
 *----------------------------------------------------------------------------*/
void configureGPIOinput(void) {
	SIM->SCGC5 |=  SIM_SCGC5_PORTD_MASK; /* enable clock for port D */

	/* Select GPIO and enable pull-up resistors and interrupts 
		on falling edges for pins connected to switches */
	PORTD->PCR[BUTTON_POS] |= PORT_PCR_MUX(1) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_IRQC(0x0a);
		
	/* Set port D switch bit to inputs */
	PTD->PDDR &= ~MASK(BUTTON_POS);

	/* Enable Interrupts */
	NVIC_SetPriority(PORTD_IRQn, 128); // 0, 64, 128 or 192
	NVIC_ClearPendingIRQ(PORTD_IRQn);  // clear any pending interrupts
	NVIC_EnableIRQ(PORTD_IRQn);
}

/*----------------------------------------------------------------------------
 * Interrupt Handler
 *    - Clear the pending request
 *    - Test the bit for pin 6 to see if it generated the interrupt 
  ---------------------------------------------------------------------------- */
void PORTD_IRQHandler(void) {  
	NVIC_ClearPendingIRQ(PORTD_IRQn);
	#if OSC_INTERRUPT_COUNTER
	oscToogle();
	#endif
	if ((PORTD->ISFR & MASK(BUTTON_POS))) {
		// CODE HERE
	}
	// Clear status flags 
	PORTD->ISFR = 0xffffffff; 
	    // Ok to clear all since this handler is for all of Port D
	#if OSC_INTERRUPT_COUNTER
	oscToogle();
	#endif
}
/*----------------------------------------------------------------------------
 * Configure SysTick to interrupt every millisecond  
 *---------------------------------------------------------------------------- */ 
void Init_SysTick(void) {
   uint32_t r = 0 ; 
   r = SysTick_Config(SystemCoreClock / 1000) ;

   // Check return code for errors 
   if (r != 0) { 
       // Error Handling - program gets stuck here
       while(1); 
   }
	 
	 SysTickCounter = TICKS_T;
}

/*----------------------------------------------------------------------------
 * Handle the SysTick interrupt
 *    Decrement the SysTick counter if > 0
 *---------------------------------------------------------------------------- */ 
void SysTick_Handler(void) {
	if (SysTickCounter > 0) 
	{
		SysTickCounter--;
	}	
}

/*----------------------------------------------------------------------------
 * nextRand: get next random number 
 *   Based on https://en.wikipedia.org/wiki/Linear_congruential_generator
 * --------------------------------------------------------------------------- */
// Returns a 32 bit number which is too long for us
uint32_t nextRand(void) {
  seed = (1103515245 * seed + 12345) ; 
  return seed ; 
}


/*----------------------------------------------------------------------------
  Task 1: flashRed

  Flash the red LED using RED_FLASH_COUNT
 *----------------------------------------------------------------------------*/
void task1FlashRed(void) {
	if (redCounter > 0) redCounter-- ;
	
	// Check counter and toggle LED
	if (redCounter==0) {
		if (redLedOn) {
			redOff() ;
			redLedOn = 0 ;
		} else {
			redOn() ;
			redLedOn = 1 ;
		}
  	redCounter = RED_FLASH_COUNT ;
	}
}

/*----------------------------------------------------------------------------
  Task 2: flashGreen

  Flash the green LED using GREED_FLASH_COUNT
 *----------------------------------------------------------------------------*/
void task2FlashGreen(void) {
	if (greenCounter > 0) greenCounter-- ;
	
	// Check counter and toggle LED
	if (greenCounter==0) {
		if (greenLedOn) {
			greenOff() ;
			greenLedOn = 0 ;
		} else {
			greenOn() ;
			greenLedOn = 1 ;
		}
  	greenCounter = GREEN_FLASH_COUNT ;
	}
}
void WaitSysTickCounter(int ticks) 
{
	/*--------------------------------------------------------------
	 * Wait until the SysTickCounter be empty, after all tasks of 
	 * the cycle have been completed
	---------------------------------------------------------------*/
	while(SysTickCounter > 0) ;
	
	/*--------------------------------------------------------------
	Reset SysTickCounter to be used again
	---------------------------------------------------------------*/
	SysTickCounter = ticks;
}

/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/
int main (void) {
	configureGPIOoutput() ;
	configureGPIOinput() ;
  Init_SysTick();
	
	// Code for flashing the LEDs	
	while (1) {
			#if OSC_TASK_COUNTER
		  oscToogle();
			#endif
	  	task1FlashRed();
		  task2FlashGreen() ;
		
			#if OSC_TASK_COUNTER
		  oscToogle();
			#endif
		
			#if OSC_CYCLE_COUNTER
			oscToogle();
			#endif
		  WaitSysTickCounter(50) ;     // Delay before repeating cycle
			// Delay(50);
	}
}
