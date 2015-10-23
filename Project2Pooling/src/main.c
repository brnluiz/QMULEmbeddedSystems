#include <MKL25Z4.H>
#include "../include/gpio_defs.h"
#include "../include/settings.h"
#include <stdbool.h>

/* ------------------------------------------
       ECS7642U Lab for week 2
   External GPIO input and output
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

  Configure the port B pin for the on-board red led as an output
 *----------------------------------------------------------------------------*/
void configureGPIOoutput() {
		// Configuration steps
	//   1. Enable clock to GPIO ports
	//   2. Enable GPIO ports
	//   3. Set GPIO direction to output
	//   4. Ensure LEDs are off

	// Enable clock to ports B 
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK ;
	
	// Make the pin GPIO
	PORTE->PCR[RED_LED_POS] &= ~PORT_PCR_MUX_MASK;          
	PORTE->PCR[RED_LED_POS] |= PORT_PCR_MUX(1);          
	
	// Set ports to outputs
	PTE->PDDR |= MASK(RED_LED_POS) ;

	// Turn off the LED
	PTE->PSOR = MASK(RED_LED_POS) ;
	
	// end of configuration code 
}

/*----------------------------------------------------------------------------
  GPIO Input Configuration

  Initialse a Port D pin as an input, with no interrupt
  Bit number given by BUTTON_POS
 *----------------------------------------------------------------------------*/
// 
void configureGPIOinput(void) {
	SIM->SCGC5 |=  SIM_SCGC5_PORTD_MASK; /* enable clock for port D */

	/* Select GPIO and enable pull-up resistors and no interrupts */
	PORTD->PCR[BUTTON_POS] |= PORT_PCR_MUX(1) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_IRQC(0x0);
	
	/* Set port D switch bit to inputs */
	PTD->PDDR &= ~MASK(BUTTON_POS);

}

/*----------------------------------------------------------------------------
  redOn: Set red LED on

  Note use of the clear register (c.f. the data output)
 *----------------------------------------------------------------------------*/
void redOn(void)
{
	   // set red on without changing anything else
	   // LED is actve low
		 PTE->PCOR |= MASK(RED_LED_POS) ;
}

/*----------------------------------------------------------------------------
  redOff: Set red LED off

  Note use of the clear register (c.f. the data output)
 *----------------------------------------------------------------------------*/
void redOff(void)
{
	   // set red off with changing anything else
	   // LED is actve low
		 PTE->PSOR |= MASK(RED_LED_POS) ;
}

/*----------------------------------------------------------------------------
  isPressed: test the switch

  Operating the switch connects the input to ground. A non-zero value
  shows the switch is not pressed.
 *----------------------------------------------------------------------------*/
bool isPressed(void) {
	if (PTD->PDIR & MASK(BUTTON_POS)) {
			return false ;
	}
	
	return true ;
}


/*----------------------------------------------------------------------------
  oneCycle

  Flash the red LED once and then test the switch
  Delay if the switch is being prssed
 *----------------------------------------------------------------------------*/
void oneCycle(void)
{
	   redOn();
		 
		 // wait for 500ms
		 Delay(500) ;
		 
	   redOff();
		 
		 // wait for 500ms
		 Delay(500) ;
}




// ----------------------------------------------------------------------------
//
//
// USER CODE
//
//
// ----------------------------------------------------------------------------

bool buttonState   = false;
bool buttonPressed = false;
bool lightOn       = false;
int  ticks         = 0;
int  blink_time    = BLINK_T;

/*----------------------------------------------------------------------------
  Blink the LED until the number of specified ticks (at the var blink_time)
 *---------------------------------------------------------------------------*/
void taskBlink(void)
{
	if (ticks > blink_time) {
		lightOn = !lightOn;
		ticks = 0;
	} else {
		ticks++;
	}
}

/*----------------------------------------------------------------------------
  Turn on the light when lightOn var is true and turn off when it is false
 *---------------------------------------------------------------------------*/
void taskLight(void)
{
	if (lightOn) {
		redOn();
	} else {
		redOff();
	}
}

/*----------------------------------------------------------------------------
  Change the blink time depending on the buttonState
 *---------------------------------------------------------------------------*/
void taskButton() {
	if (isPressed()) {
		
		if (!buttonPressed) {
			buttonPressed = true;
			buttonState = !buttonState;
			
			// Configure the blinks as specified
			if (buttonState) {
				blink_time = BLINK_T * 4;
			} else {
				blink_time = BLINK_T / 4;
			}
		}
	} else {
		buttonPressed = false;
	}
}

/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/
int main (void) {
	configureGPIOoutput() ;
	configureGPIOinput() ;
  
	// Code for flashing the LEDs	
	while (1) {		
		taskBlink();
		taskLight();
		taskButton();
		Delay(DELAY_T);
	}
}
