#ifndef SETTINGS_H
#define SETTINGS_H

enum {
	LED_BLINKING,
	LED_NOT_BLINKING
};

enum {
	EVT_BTN_PRESSED = 1
};

enum {
	COLOR_RED,
	COLOR_GREEN,
	COLOR_BLUE
};

#define LED_TIMEOUT 280 // 2800mS / 10mS (ticks) - Considering a delay of 20ms for debouncing

#endif
