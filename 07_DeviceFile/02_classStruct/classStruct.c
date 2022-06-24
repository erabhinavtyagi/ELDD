// Creating Device File Automatically

#include<linux/init.h>
#include<linux/module.h>
#include<linux/kdev_t.h>
#include<linux/fs.h>
#include<linux/device.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("ABHINAV TYAGI");
MODULE_DESCRIPTION("Creating Device File Automatically Using Struct Class.");


dev_t dev = 0;
static struct class *dev_class;

// MODULE INITIALIZATION

static int __init devicefile_init(void)
{
	//allocating Major Number
	if((alloc_chrdev_region(&dev, 0, 1, "CharDevClass")) <0)
	{
		pr_err("Can not allocate Major Number for your Device.\n");
		return -1;
	}

	//Creating Struct Class
	if ((dev_class = class_create(THIS_MODULE,"CharDevClass")) == NULL)
	{
		pr_err("Can not create the Struct Class for your Device.\n");
		goto r_class;
	}

	//Creating Device
	if ((device_create(dev_class,NULL,dev,NULL,"CharDevClass")) == NULL)
        {
                pr_err("Can not create the Device.\n");
                goto r_device;
        }
	
	pr_info("Kernel Module Inserted Successfully.\n");
	return 0;

r_device:
	class_destroy(dev_class);

r_class:
	unregister_chrdev_region(dev,1);
	return -1;
}

// MODULE EXIT FUNCTION

static void __exit devicefile_exit(void)
{
	device_destroy(dev_class, dev);
	class_destroy(dev_class);
	unregister_chrdev_region(dev,1);
	pr_info("Kernel Module Removed Successfully.\n");
}

module_init(devicefile_init);
module_exit(devicefile_exit);
