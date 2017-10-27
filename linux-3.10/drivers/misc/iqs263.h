/*
 *	iqs263.h - IQS263 Registers and Bit Definitions
 *
 *	Copyright (C) 2016 iWave Ltd
 *	Author: Hai Peng Yu <yuhp@iwave.cc>
 *
 *  This program is free software; you can redistribute  it and/or modify it
 *  under  the terms of  the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the  License, or (at your
 *  option) any later version.
 *
 *	iWave Ltd does not take responsibility for the use of this driver.
 *
 *	This header file contains the register and bit definitions for the
 *	IQS263 SAR Sensor, to be used in the driver. This makes the code
 *	cleaner to read.
 */
#ifndef IQS263_H
#define IQS263_H

#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/err.h>
//#include <linux/hrtimer.h>
#include <linux/gpio.h>
#include <linux/of_gpio.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/init-input.h>
#include <linux/of.h>
#include <linux/sys_config.h>
#include <linux/pinctrl/consumer.h>
#include <linux/pinctrl/pinconf-sunxi.h>
#include <linux/regulator/consumer.h>
#include <linux/i2c.h>
#include <linux/input.h>
#include <linux/types.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/proc_fs.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
//#include <linux/time.h>





/*	Definitions for the driver	*/
#define IQS_MAJOR				248
#define IQS_MINOR				0
#define IQS_DEV_NUMS			1
#define DEVICE_NAME             "iqs263"
#define IQS263_DRIVER_NAME      "iqs263_drv"

/*	Addresses of Registers on IQS263	*/
/*	Read	*/
#define DEVICE_INFO             0x00
#define SYS_FLAGS               0x01
#define COORDINATES             0x02
#define TOUCH_BYTES             0x03
#define COUNTS                  0x04
#define LTA                     0x05
#define DELTAS                  0x06
#define MULTIPLIERS             0x07
#define COMPENSATION            0x08
#define PROX_SETTINGS           0x09
#define THRESHOLDS              0x0A
#define TIMINGS_AND_TARGETS     0x0B
#define GESTURE_TIMERS          0x0C
#define ACTIVE_CHANNELS         0x0D
//#define PROX_SETTINGS0			    0x09

typedef union {
	int val;
	const char *str;
	const char *name;
	struct gpio_config gpio;
}script_item_ufm;

struct iqs263_param{
	unsigned char iqs263_used;
	unsigned char iqs263_twi_id;
	const char* iqs263_power_ldo;
	const char* iqs263_name;
	int iqs263_power_vol;
	unsigned int iqs263_rst_sys;
}iqs263_para;



/*	BIT DEFINITIONS FOR IQS263	*/

/*	Bit definitions	*/
/*	System Flags - STATUS	*/
/*	Indicates ATI is busy	*/
#define	ATI_BUSY			0x04
/*	Indicates reset has occurred	*/
#define	SHOW_RESET			0x80
/*	Bit definitions - Event Bytes	*/
#define	PROX_EVENT			    0x01
#define	TOUCH_EVENT			    0x02
#define	SLIDE_EVENT			    0x04
#define	MOVE_EVENT			    0x10
#define	TAP_EVENT			    0x20
#define	FLICKLEFT_EVENT			0x40
#define	FLICKRIGHT_EVENT		0x80

//Touch
#define IQS263_TOUCH_CH0              0x0001  //PROX
#define IQS263_TOUCH_CH1              0x0002
#define IQS263_TOUCH_CH2              0x0004
#define IQS263_TOUCH_CH3              0x0008

/*	Bit definitions - ProxSettings0	*/
/*	Reseed the IQS253	*/
#define	RESEED				0x08
/*	Redo ATI - switch on ATI	*/
#define	REDO_ATI			0x10
/*	0 - Partial ATI Off, 1 - Partial ATI On	*/
#define	ATI_PARTIAL			0x40
/*	0 - ATI Enabled, 1 - ATI Disabled	*/
#define	ATI_OFF				0x80


/*	Set IQS263 in Projected mode	*/
#define	PROJ_MODE			0x10

/*	Bit definitions - ProxSettings1	*/
/*	0-Streaming Mode, 1-Event Mode	*/
#define	EVENT_MODE			0x40


/*	Delays	*/
#define HANDSHAKE_DELAY_HOLD	11		/*	11ms	*/
#define HANDSHAKE_DELAY_SET		200		/*	200μs	*/

/*	Acknowledge that chip was reset	*/
#define ACK_RESET			0x80


/* Change the Thresholds for each channel (0x0A in this order) */
#define PROX_THRESHOLD	  0x04
//#define TOUCH_THRESHOLD  0x20
#define TOUCH_THRESHOLD_CH1  0x30
#define TOUCH_THRESHOLD_CH2  0x30
#define TOUCH_THRESHOLD_CH3  0x10
#define MOVEMENT_THRESHOLD  0x03
#define RESEED_BLOCK  0x00
#define HALT_TIME  0x14
#define I2C_TIMEOUT  0x04

/* Change the Gesture Timing settings (0x0C in this order) */
#define TAP_TIMER 0x00
#define FLICK_TIMER  0x5A
#define FLICK_THRESHOLD  0x1B

/* Set Active Channels (0x0D) */
#define ACTIVE_CHS 0x0F //only CH0 CH1 CH2

/* Change the Timing settings (0x0B in this order) */
#define ATI_TARGET_TOUCH  0x20
#define ATI_TARGET_PROX  0x20

#endif
