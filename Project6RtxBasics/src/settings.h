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

#define LED_TIMEOUT      300 // 3000ms / 10ms (ticks)
#define DEBOUNCE_TIMEOUT 20  // 200ms
#endif
