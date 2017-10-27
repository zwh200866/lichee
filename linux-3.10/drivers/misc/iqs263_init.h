#ifndef IQS263_INIT_H
#define IQS263_INIT_H


/* Used to switch Projected mode & set Global Filter Halt (0x01 Byte1) */
#define SYSTEM_FLAGS_VAL					0x00
/* Enable / Disable system events (0x01 Byte2)*/
#define SYSTEM_EVENTS_VAL					0x00

/* Change the Multipliers & Base value (0x07 in this order) */
#define MULTIPLIERS_CH0						0x1B
#define MULTIPLIERS_CH1						0x15
#define MULTIPLIERS_CH2						0x15
#define MULTIPLIERS_CH3						0x0F
#define BASE_VAL						    0x44

/* Change the Compensation for each channel (0x08 in this order) */
#define COMPENSATION_CH0					0x54
#define COMPENSATION_CH1					0x4A
#define COMPENSATION_CH2					0x4C
#define COMPENSATION_CH3					0x49

/* Change the Prox Settings or setup of the IQS263 (0x09 in this order) */
#define PROXSETTINGS0_VAL					0x00
#define PROXSETTINGS1_VAL					0x11 
#define PROXSETTINGS2_VAL					0x00
#define PROXSETTINGS3_VAL					0x00
#define EVENT_MASK_VAL						0x03



/* Change the Timing settings (0x0B in this order) */
#define LOW_POWER						      0x00

#endif	/* IQS253_INIT_H */
