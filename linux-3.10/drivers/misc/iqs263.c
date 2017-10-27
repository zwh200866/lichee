#include "iqs263.h"
#include "iqs263_init.h"

static __u32 twi_id = 0;
/*	i2c slave device address	*/
#define IQS263_ADDR	0x44
static const unsigned short normal_i2c[2] = {IQS263_ADDR, I2C_CLIENT_END};

//added by justin
static u8 currentKey = 0;	//added by justin
const u16 iqs263_key_array[]={
	KEY_VOLUMEUP,//115
	KEY_VOLUMEDOWN,//114
	KEY_BACK,
	KEY_MENU,//139
};

static bool debug;

#define dprintk(fmt, args...)	\
	do {							          \
		if (debug)					      \
			printk(KERN_DEBUG DEVICE_NAME ": " fmt, ## args);\
	} while (0)
struct regulator *iqs263_regulator = NULL;

static u32 RDY_LINE_PIN = GPIOH(11);

/* Boolean value used for the initial setup */
static bool doInitialSetup = true;
/*	Boolean to keep track of Chip Reset Events	*/
//static bool showReset=false;

//static bool reseed;

/*	Global variable to keep the currentState in	*/
//static u8 currentState;	/*	Always start at state 0	*/

/*	Each client has this additional data	*/
struct iqs263_sar_data {
	struct i2c_client *client;
	struct input_dev *input_dev;
	const struct iqs263_sar_platform_data *platform_data;
};
//static int iqs263_request_irq(struct i2c_client *client);

/*	Events data Struct	*/
struct events_data{
	/*	Boolean to keep track of Prox   Events	*/
	bool prox;
	bool touch;
	bool touchleft;
	
	bool touchright;
};
//#############需要注意的几点:   ##############
//此处不能添加打印，因为打印需要占用时间，
//可能会导致超过了RDY脚的低电平，导致I2C通信出错



/********************************************************
 *		The IQS263 specific setup is done here			*
 *		This setup will change for each application		*
 ********************************************************/
/*	Command for reseeding the Azoteq IQS263	*/
static void __maybe_unused iqs263_reseed(struct iqs263_sar_data *data)
{
	i2c_smbus_write_byte_data(data->client, PROX_SETTINGS, RESEED);
}

/*	Command for re-doing ATI on the Azoteq IQS263	*/
static void __maybe_unused iqs263_reati(struct iqs263_sar_data *data)
{
	/*	Redo ATI	*/
	i2c_smbus_write_byte_data(data->client, PROX_SETTINGS, REDO_ATI);
}


static int iqs263_i2c_read(struct i2c_client *client, u8 *buf)
{
	int ret,i;
	u8 reg1,reg2;
	reg1 = SYS_FLAGS;
	reg2 = TOUCH_BYTES;
	
	struct i2c_msg msgs[4] = {
		{
			.addr	= client->addr,
			.flags	= !I2C_M_RD, //write reg
			.len	= 1,
			.buf	= &reg1,
		},
		{
			.addr	= client->addr,
			.flags	= I2C_M_RD,
			.len	= 2,
			.buf	= buf,
		},

		{
			.addr	= client->addr,
			.flags	= !I2C_M_RD, //write reg
			.len	= 1,
			.buf	= &reg2,
		},
		{
			.addr	= client->addr,
			.flags	= I2C_M_RD,
			.len	= 2,
			.buf	= buf+2,
		},
	};
	
	ret = i2c_transfer(client->adapter, msgs, 4);
		
	if (ret < 0){
		printk("%s error: %d\n", __func__, ret);
	}else{
		printk("r_buf:");
		for(i=0;i<4;i++){
			printk("[%d]=%x,",i,buf[i]);
		}
		printk("\n");
	}

	return ret;
}

static int iqs263_i2c_read_ati(struct i2c_client *client, u8 *buf)
{
	int ret;
	u8 reg1;
	reg1 = SYS_FLAGS;
	//reg2 = TOUCH_BYTES;
	
	struct i2c_msg msgs[2] = {
		{
			.addr	= client->addr,
			.flags	= !I2C_M_RD, //write reg
			.len	= 1,
			.buf	= &reg1,
		},
		{
			.addr	= client->addr,
			.flags	= I2C_M_RD,
			.len	= 1,
			.buf	= buf,
		},
	};
	
	ret = i2c_transfer(client->adapter, msgs, 2);
		
	if (ret < 0){
		printk("%s error: %d\n", __func__, ret);
	}else{
		printk("reg1=0x%x\n",buf[0]);
	}

	return ret;
}

static u8 iqs_init_buf[]={
	SYS_FLAGS,SYSTEM_FLAGS_VAL,\
	ACTIVE_CHANNELS,ACTIVE_CHS,\
	THRESHOLDS,PROX_THRESHOLD,TOUCH_THRESHOLD_CH1,TOUCH_THRESHOLD_CH2,TOUCH_THRESHOLD_CH3,MOVEMENT_THRESHOLD,RESEED_BLOCK,HALT_TIME,I2C_TIMEOUT,\
	TIMINGS_AND_TARGETS,LOW_POWER,ATI_TARGET_TOUCH,ATI_TARGET_PROX,\
	MULTIPLIERS,MULTIPLIERS_CH0,MULTIPLIERS_CH1,MULTIPLIERS_CH2,MULTIPLIERS_CH3,\
	PROX_SETTINGS,PROXSETTINGS0_VAL,PROXSETTINGS1_VAL,PROXSETTINGS2_VAL,PROXSETTINGS3_VAL,EVENT_MASK_VAL,\
	COMPENSATION,COMPENSATION_CH0,COMPENSATION_CH1,COMPENSATION_CH2,COMPENSATION_CH3,\
	GESTURE_TIMERS,TAP_TIMER,FLICK_TIMER,FLICK_THRESHOLD,\
	PROX_SETTINGS,REDO_ATI,
	PROX_SETTINGS,PROXSETTINGS0_VAL,(PROXSETTINGS1_VAL|EVENT_MODE),PROXSETTINGS2_VAL,PROXSETTINGS3_VAL,EVENT_MASK_VAL,
};

static int cur_init_setup_idx=0;
struct i2c_msg iqs_init_msg[]={
	{
		.addr	= IQS263_ADDR,
		.flags   = 0, //write
		.len	= 2,
		.buf	= &iqs_init_buf[0],
	},
	{
		.addr	= IQS263_ADDR,
		.flags	= 0,
		.len	= 2,
		.buf	= &iqs_init_buf[2],
	},	
	{
		.addr	= IQS263_ADDR,
		.flags	= 0,
		.len	= 9,
		.buf	= &iqs_init_buf[4],
	},
	{
		.addr	= IQS263_ADDR,
		.flags	= 0,
		.len	= 4,
		.buf	= &iqs_init_buf[13],
	},
	{
		.addr	= IQS263_ADDR,
		.flags	= 0,
		.len	= 5,
		.buf	= &iqs_init_buf[17],
	},
	{
		.addr	= IQS263_ADDR,
		.flags	= 0,
		.len	= 6,
		.buf	= &iqs_init_buf[22],
	},
	{
		.addr	= IQS263_ADDR,
		.flags	= 0,
		.len	= 5,
		.buf	= &iqs_init_buf[28],
	},
	{
		.addr	= IQS263_ADDR,
		.flags	= 0,
		.len	= 4,
		.buf	= &iqs_init_buf[33],
	},
	{
		.addr	= IQS263_ADDR,
		.flags	= 0,
		.len	= 2,
		.buf	= &iqs_init_buf[37],
	},
};
struct i2c_msg iqs_eventmode_msg[]={
	{
		.addr	= IQS263_ADDR,
		.flags	= 0,
		.len	= 6,
		.buf	= &iqs_init_buf[39],
	}
};

static int iqs263_detect(struct i2c_client *client, struct i2c_board_info *info)
{
	struct i2c_adapter *adapter = client->adapter;
	//int ret;
	if (!i2c_check_functionality(adapter, I2C_FUNC_SMBUS_BYTE_DATA)){
        return -ENODEV;
	}
					
	if(twi_id == adapter->nr){
        printk("%s ok!\n",__FUNCTION__);
        strlcpy(info->type, DEVICE_NAME, I2C_NAME_SIZE);
		return 0;	
	}else{
		return -ENODEV;
	}
}

struct iqs263_sar_platform_data {
	void (*cfg_pin)(void);
};


/**
 *	Because the IQS263 operates in event mode, implement a handshake
 *	function that will initiate comms with the IQS263 if we want to talk
 *	to it.
 */
 //added by justin: 当需要读写IQS263相关寄存器时，可以执行此函数，
 //IQS263就会自动地给主控一个IRQ(RDY脚中断)，然后主控就可以执行读写操作了
//但A64的中断脚在中断模式和输出IO切换时，好像行不通
//故不执行此函数
void __maybe_unused iqs263_event_mode_handshake(struct iqs263_sar_data *data)
{
	/********************************************************
	 *			Try and do an Event Mode Handshake			*
	 *******************************************************/
	/*
	 *	There might be another way to build in the delay.
	 *	Event mode handshake is done by manually pulling
	 *	the IQS263 RDY line low for ~10ms and then releasing
	 *	it. This will tell the IQS263 that the Master wants
	 *	to talk and it will then give a Communications window
	 *	which will call the interrupt request function.
	 *	From there the IQS263 can be read again
	 */
	/*	Pull RDY low	*/
	//u8 data_buffer[2];
	printk(KERN_ALERT"~~~%s start\n", __FUNCTION__);

	gpio_direction_output(RDY_LINE_PIN, 0);
	/*	Hold the RDY low for ~10ms	*/
	mdelay(HANDSHAKE_DELAY_HOLD);
	/*	Release RDY line	*/
	gpio_direction_input(RDY_LINE_PIN);
	/*	Delay before talking to IQS263	*/
	udelay(HANDSHAKE_DELAY_SET);
	printk(KERN_ALERT"~~~%s end\n", __FUNCTION__);
}


/************************************************************************
 *							IQS263 Initial Setup						*
 ************************************************************************/
/* Setup the Active Channels for Azoteq IQS263 */
static void iqs263_init_setup(struct iqs263_sar_data *data)
{
	int ret;
	u8 buf[2];

	if(cur_init_setup_idx == 0){
		ret = i2c_transfer(data->client->adapter, iqs_init_msg, sizeof(iqs_init_msg)/sizeof(struct i2c_msg)-1);//do not send event mode msg
		if (ret < 0){
			printk("%s %d, Error:%d\n", __func__, __LINE__,ret);
		}
		else{
			cur_init_setup_idx++;
		}
	}
	else if(cur_init_setup_idx == 1){//read ATI is ready
		if(iqs263_i2c_read_ati(data->client, buf)>=0){
			if(((buf[0] & ATI_BUSY) == 0)){
				cur_init_setup_idx++;
			}
		}
	}
	else if(cur_init_setup_idx == 2){//set to event mode
		i2c_transfer(data->client->adapter, iqs_eventmode_msg, 1);
		if (ret < 0){
			printk("%s %d, Error:%d\n", __func__, __LINE__,ret);
		}
		else{
			doInitialSetup = false;
			cur_init_setup_idx=0;
			printk("%s Ok: %d\n", __func__, ret);
		}
	}
	return;
}


static bool readEvents(struct iqs263_sar_data *data, u8 *data_buffer)
{
	//printk(KERN_ALERT"~~~~~~~ dirk dbg, %s %d\n",__FUNCTION__,__LINE__);
	int ret;
	
	ret = iqs263_i2c_read(data->client, data_buffer);
	if(ret < 0){
		printk(KERN_ALERT"err! F=%s,L=%d\n",__FUNCTION__,__LINE__);
		return false;
	}
	return true;
}

//中断时读写I2C前，不能打印, 读写完I2C后才能打印
//因为打印完后，可能超过了RDY脚的低电平了
static irqreturn_t iqs263_sar_interrupt(int irq, void *dev_id)
{
	struct iqs263_sar_data *data = dev_id;
	u8 data_buffer[32];

	if (doInitialSetup) {
		iqs263_init_setup(data);
		goto out;
	}
	
	/*	Check Events	*/
	if(!readEvents(data, data_buffer)){
		printk("err! %s,%d\n", __FUNCTION__, __LINE__);
		goto out;
	}
	
	if (data_buffer[0] & SHOW_RESET){
		doInitialSetup = true;
		printk("err! %s,%d\n", __FUNCTION__, __LINE__);
		goto out;
	}
	else{
		doInitialSetup = false;
	}
	
	if(data_buffer[1] & TOUCH_EVENT) {//忽略PROX功能
		if(data_buffer[2] & IQS263_TOUCH_CH1){
			currentKey = iqs263_key_array[0];
			input_report_key(data->input_dev, currentKey, 1);
			printk(KERN_ALERT"key1\n");
			input_sync(data->input_dev);
		}
		else if(data_buffer[2] & IQS263_TOUCH_CH2){
			currentKey = iqs263_key_array[1];
			input_report_key(data->input_dev, currentKey, 1);
			printk(KERN_ALERT"key2\n");
			input_sync(data->input_dev);
		}
		else if(data_buffer[2] & IQS263_TOUCH_CH3){
			currentKey = iqs263_key_array[2];
			input_report_key(data->input_dev, currentKey, 1);
			printk(KERN_ALERT"key3\n");
			input_sync(data->input_dev);
		}
		else if((data_buffer[2] & (IQS263_TOUCH_CH1|IQS263_TOUCH_CH2|IQS263_TOUCH_CH3))==0){
			if(currentKey){
				input_report_key(data->input_dev, currentKey, 0);//key up
				printk(KERN_ALERT"keyup%d\n",(currentKey==iqs263_key_array[0])?1:((currentKey==iqs263_key_array[1])?2:((currentKey==iqs263_key_array[2])?3:0)));
				currentKey = 0;
				input_sync(data->input_dev);
			}
		}
	}
	else if(data_buffer[1] & PROX_EVENT){//only get PROX event
		if(data_buffer[2] & IQS263_TOUCH_CH0){
			currentKey = iqs263_key_array[3];
			input_report_key(data->input_dev, currentKey, 1);
			printk(KERN_ALERT"prox\n");
			input_sync(data->input_dev);
		}
		else if(currentKey){
			input_report_key(data->input_dev, currentKey, 0);//key up
			currentKey = 0;
			printk(KERN_ALERT"proxUp\n");
			input_sync(data->input_dev);
		}
	}

out:
	return IRQ_HANDLED;
}



static int iqs263_sar_probe(struct i2c_client *client,const struct i2c_device_id *id)
{
	struct iqs263_sar_data *data;
	struct input_dev *input_dev;
	int ret;
	printk(KERN_ALERT"~~ call %s\n", __FUNCTION__);

	data = kzalloc(sizeof(struct iqs263_sar_data), GFP_KERNEL);
	input_dev = input_allocate_device();
	if (!data || !input_dev) {
		dev_err(&client->dev, "Failed to allocate memory\n");
		ret = -ENOMEM;
		goto err_free_mem;
	}

	/*	Save the stuctures to be used in the driver	*/
	data->client = client;
	//client_globle = client;
	data->input_dev = input_dev;
	data->platform_data = client->dev.platform_data;

	input_dev->name = "iqs263";
	input_dev->id.bustype = BUS_I2C;
	input_dev->dev.parent = &client->dev;

	//set_bit(EV_SYN, input_dev->evbit);
	set_bit(EV_KEY, input_dev->evbit);
	//set_bit(BTN_TOUCH, input_dev->keybit);
	set_bit(KEY_VOLUMEDOWN, input_dev->keybit);
	set_bit(KEY_VOLUMEUP, input_dev->keybit);
	set_bit(KEY_BACK, input_dev->keybit);
	set_bit(KEY_MENU, input_dev->keybit);
	/*	Request the RDY line for the IQS263	*/
	iqs263_event_mode_handshake(data);


	client->irq = gpio_to_irq(RDY_LINE_PIN);
	/* Set driver data */
	input_set_drvdata(input_dev, data);
	/*
	 *	Request the interrupt on a falling trigger
	 *	and only one trigger per falling edge
	 */
	ret = request_threaded_irq(client->irq, NULL, iqs263_sar_interrupt,
	              IRQF_TRIGGER_FALLING | IRQF_ONESHOT, IQS263_DRIVER_NAME, data);

	switch (ret) 
	{
		case -EBUSY:
			printk(KERN_ERR IQS263_DRIVER_NAME": IRQ %d is busy\n",client->irq);
			goto exit_gpio_free_rdy_pin;
		case -EINVAL:
			printk(KERN_ERR IQS263_DRIVER_NAME": Bad irq number or handler\n");
			goto exit_gpio_free_rdy_pin;
		default:
		    printk("Interrupt %d obtained\n",client->irq);
			break;
	};

	/*	Register the device */
	ret = input_register_device(data->input_dev);
	if (ret < 0) {
		dev_err(&client->dev, "Failed to register interrupt\n");
		goto exit_gpio_free_rdy_pin;
	}
	/*	Now, assign default values to global variables	*/
	/*	Periodically check for chip reset	*/
	//currentState = 0;	/*	Always start at state 0	*/
	//reseed = false;
	//showReset = false;

	
	i2c_set_clientdata(client, data);
	printk(KERN_ALERT"~~~%s end\n", __FUNCTION__);
	return 0;

exit_gpio_free_rdy_pin:
	printk(KERN_ALERT"~~~%s err\n", __FUNCTION__);
	gpio_free(RDY_LINE_PIN);
err_free_mem:
	printk(KERN_ALERT"~~~%s err\n", __FUNCTION__);
	input_free_device(input_dev);
	kfree(data);
	return ret;
}


static int iqs263_sar_remove(struct i2c_client *client)
{
	struct iqs263_sar_data *data = i2c_get_clientdata(client);
	printk(KERN_ALERT"~~~call %s\n", __FUNCTION__);

	free_irq(client->irq, data);

	input_unregister_device(data->input_dev);
	input_free_device(data->input_dev);
	kfree(data);
	gpio_free(RDY_LINE_PIN);
	//gpio_free(RDY_LINE_PIN2);
	//RDY_LINE_PIN_is_inited=0;

	i2c_set_clientdata(client, NULL);

	return 0;
}

/*	Standard stucture with the device id for identification	*/
static const struct i2c_device_id iqs263_sar_id[] = {
		{ DEVICE_NAME, 0 },
		{ }
};

MODULE_DEVICE_TABLE(i2c, iqs263_sar_id);

#ifdef CONFIG_OF
static struct of_device_id iqs263_match_table[] = {
        { .compatible = "azoteq,iqs263",},
        { },
};
#else
#define iqs263_match_table NULL
#endif

static struct i2c_driver iqs263_sar_driver = {
	.class = I2C_CLASS_HWMON,
	.probe = iqs263_sar_probe,
    .remove = iqs263_sar_remove,
	.driver = {
		.name = DEVICE_NAME,
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(iqs263_match_table),
	},
	.id_table = iqs263_sar_id,
	.address_list = normal_i2c,
	.detect = iqs263_detect,
};

static int iqs263_gpio_init(void)
{
	//printk("## %s start ##\n",__FUNCTION__);
	gpio_request(RDY_LINE_PIN, "interrupt");
	//printk("## %s end ##\n",__FUNCTION__);
	return 0;
}

static int __init iqs263_sar_init(void)
{
	int ret = -1;
	printk("======= iqs263 init start ========\n");
	ret = iqs263_gpio_init();
	if(ret < 0){
		printk("err! %s,%d, ret = %d\n", __FUNCTION__, __LINE__, ret);
	}
	msleep(50);

	ret = i2c_add_driver(&iqs263_sar_driver);
	printk("============= iqs263 init end ==== =====\n");
	return ret;

}

static void __exit iqs263_sar_exit(void)
{
	doInitialSetup = true;
	/*	Delete driver	*/
	i2c_del_driver(&iqs263_sar_driver);
	printk(KERN_ALERT "Delete: - %s\n","IQS263 SAR Sensor driver Deleted! ");
}

module_init(iqs263_sar_init);
module_exit(iqs263_sar_exit);

/* Module information */
MODULE_DESCRIPTION("SAR Sensor driver for Azoteq IQS263");
MODULE_LICENSE("GPL");

module_param(debug, bool, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(debug, "Enable debugging messages");

