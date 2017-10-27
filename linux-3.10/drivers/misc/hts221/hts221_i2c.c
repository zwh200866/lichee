/*
 * STMicroelectronics hts221_i2c driver
 *
 * Copyright 2016 STMicroelectronics Inc.
 *
 * Lorenzo Bianconi <lorenzo.bianconi@st.com>
 *
 * Licensed under the GPL-2.
 */
#include <linux/i2c.h>
#include <linux/slab.h>
#include <linux/input.h>

#include "hts221_core.h"
#include <linux/delay.h>

#define I2C_AUTO_INCREMENT	0x80

#define DEVICE_NAME             "hts221"

static __u32 twi_id = 1;
/*	i2c slave device address	*/
#define HTS221_ADDR	0x5f

static const unsigned short normal_i2c[2] = {HTS221_ADDR, I2C_CLIENT_END};

/* XXX: caller must hold cdata->lock */
static int hts221_i2c_read(struct device *dev, u8 addr, int len, u8 *data)
{
	struct i2c_msg msg[2];
	struct i2c_client *client = to_i2c_client(dev);

	if (len > 1)
		addr |= I2C_AUTO_INCREMENT;

	msg[0].addr = client->addr;
	msg[0].flags = client->flags;
	msg[0].len = 1;
	msg[0].buf = &addr;

	msg[1].addr = client->addr;
	msg[1].flags = client->flags | I2C_M_RD;
	msg[1].len = len;
	msg[1].buf = data;

	return i2c_transfer(client->adapter, msg, 2);
}

/* XXX: caller must hold cdata->lock */
static int hts221_i2c_write(struct device *dev, u8 addr, int len, u8 *data)
{
	u8 send[len + 1];
	struct i2c_msg msg;
	struct i2c_client *client = to_i2c_client(dev);

	if (len > 1)
		addr |= I2C_AUTO_INCREMENT;

	send[0] = addr;
	memcpy(&send[1], data, len * sizeof(u8));

	msg.addr = client->addr;
	msg.flags = client->flags;
	msg.len = len + 1;
	msg.buf = send;

	return i2c_transfer(client->adapter, &msg, 1);
}

static const struct hts221_transfer_function hts221_i2c_tf = {
	.write = hts221_i2c_write,
	.read = hts221_i2c_read,
};

int hts221_i2c_write_bytes(struct i2c_client *client, uint8_t *data, uint16_t len)
{
	struct i2c_msg msg;
	int ret=-1;
	
	msg.flags = !I2C_M_RD;
	msg.addr = client->addr;
	msg.len = len;
	msg.buf = data;		
	
	ret=i2c_transfer(client->adapter, &msg,1);
	return ret;

}
bool hts221_i2c_test(struct i2c_client * client)
{
	int ret,retry;
	uint8_t test_data[1] = {0};	//only write a data address.

	for(retry=0; retry < 12; retry++)
	{
		ret =hts221_i2c_write_bytes(client, test_data, 1);	//Test i2c.
		if (ret == 1){
			//printk("retry================%d\n",retry);
			break;
			}
		msleep(20);
	}

	return ret==1 ? true : false;
}
static int hts221_detect(struct i2c_client *client, struct i2c_board_info *info)
{
	struct i2c_adapter *adapter = client->adapter;
	int ret;	
	if (!i2c_check_functionality(adapter, I2C_FUNC_SMBUS_BYTE_DATA))
                return -ENODEV;		
	if(twi_id == adapter->nr){
        ret = hts221_i2c_test(client);
        if(!ret){
        	return -ENODEV;
        }else{      
            printk("I2C connection sucess!\n");
            strlcpy(info->type, DEVICE_NAME, I2C_NAME_SIZE);
    		return 0;	
	    }
	}else{
		return -ENODEV;
	}

}

#ifdef CONFIG_PM
static int hts221_i2c_resume(struct device *device)
{
	struct i2c_client *client = to_i2c_client(device);
	struct hts221_dev *dev = i2c_get_clientdata(client);

	return hts221_enable(dev);
}

static int hts221_i2c_suspend(struct device *device)
{
	struct i2c_client *client = to_i2c_client(device);
	struct hts221_dev *dev = i2c_get_clientdata(client);

	return hts221_disable(dev);
}

static const struct dev_pm_ops hts221_i2c_pm_ops = {
	SET_SYSTEM_SLEEP_PM_OPS(hts221_i2c_suspend,
				hts221_i2c_resume)
};
#endif /* CONFIG_PM */

static int hts221_i2c_probe(struct i2c_client *client,
			    const struct i2c_device_id *id)
{
	printk(KERN_ALERT"~~~~~~~ my dbg, %s %d\n", __FUNCTION__,__LINE__);
	int err;
	struct hts221_dev *dev;

#ifdef HTS221_DEBUG
	dev_info(&client->dev, "probe start.\n");
#endif

	/* Alloc Common data structure */
	dev = kzalloc(sizeof(struct hts221_dev), GFP_KERNEL);
	if (!dev) {
		dev_err(&client->dev, "failed to allocate module data\n");
		return -ENOMEM;
	}

	dev->name = client->name;
	dev->bus_type = BUS_I2C;
	dev->tf = &hts221_i2c_tf;
	dev->dev = &client->dev;

	i2c_set_clientdata(client, dev);

	mutex_init(&dev->lock);
	err = hts221_probe(dev);
	if (err < 0) {
		kfree(dev);
		return err;
	}
	return 0;
}

int hts221_i2c_remove(struct i2c_client *client)
{
	struct hts221_dev *dev = i2c_get_clientdata(client);

#ifdef HTS221_DEBUG
	dev_info(&client->dev, "driver removing\n");
#endif

	hts221_remove(dev);
	kfree(dev);

	return 0;
}

static const struct i2c_device_id hts221_i2c_id[] = {
	{ DEVICE_NAME, 0 },
	{ },
};
MODULE_DEVICE_TABLE(i2c, hts221_i2c_id);

#ifdef CONFIG_OF
static const struct of_device_id hts221_i2c_id_table[] = {
	{ .compatible = "st,hts221" },
	{ },
};
MODULE_DEVICE_TABLE(of, hts221_i2c_id_table);
#endif /* CONFIG_OF */

static struct i2c_driver hts221_i2c_driver = {
	.class = I2C_CLASS_HWMON,
	.driver = {
		.owner = THIS_MODULE,
		.name = DEVICE_NAME,
#ifdef CONFIG_PM
		.pm = &hts221_i2c_pm_ops,
#endif /* CONFIG_PM */
#ifdef CONFIG_OF
		.of_match_table = hts221_i2c_id_table,
#endif /* CONFIG_OF */
	},
	.probe = hts221_i2c_probe,
	.remove = hts221_i2c_remove,
	.id_table = hts221_i2c_id,
	.address_list = normal_i2c,
	.detect = hts221_detect,
};

module_i2c_driver(hts221_i2c_driver);

MODULE_DESCRIPTION("hts221 i2c driver");
MODULE_AUTHOR("Lorenzo Bianconi <lorenzo.bianconi@st.com>");
MODULE_LICENSE("GPL v2");