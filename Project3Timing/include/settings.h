#ifndef SETTINGS_H
#define SETTINGS_H

#define TICKS_T 50

enum {
	OSC_OFF,
	OSC_ON,
};

enum {
	NOT_FLASHING,
	FLASHING,
};

enum {
	BUTTON_NOT_PRESSED,
	BUTTON_PRESSED,
};

enum {
	STATE_ARMED,
	STATE_RANDOM,
	STATE_TIMING,
	STATE_SUCCESS,
	STATE_ERROR
};


#define OSC_CYCLE_COUNTER     false
#define OSC_TASK_COUNTER      false
#define OSC_INTERRUPT_COUNTER true

#endif
