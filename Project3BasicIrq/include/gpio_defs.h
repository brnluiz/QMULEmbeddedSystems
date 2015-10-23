#ifndef GPIO_DEFS_H
#define GPIO_DEFS_H

#define MASK(x) (1UL << (x))

// Freedom KL25Z LEDs
#define RED_LED_POS (18)		// on port B
#define GREEN_LED_POS (19)	// on port B
#define BLUE_LED_POS (1)		// on port D

// Switches is on port D, pin 6
#define BUTTON_POS (6)

#define OSCIL_POS (8) // PTB8

// On / off times of flashes
#define RED_FLASH_COUNT (17) // 17 x 50ms = 850ms
#define GREEN_FLASH_COUNT (13) // 13 x 50ms = 650ms

#endif
