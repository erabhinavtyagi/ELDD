// IOCTL - Input / Output Control

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/kdev_t.h>

#include "ioctl.h"	 // user defined Header file

#define COUNT 1
#define DEV_NAME "THANOS"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ABHINAV");
MODULE_DESCRIPTION("IOCTL - Input / Output Control");

dev_t dev_num = 0;
static struct cdev chardev;

// Device open
static int chardev_open(struct inode *inode, struct file *filep)
{
	pr_info("chardev_open Invoked\n");
	return 0;
}

// Device released
static int chardev_release(struct inode *inode, struct file *filep)
{
	pr_info("chardev_release Invoked\n");
	return 0;
}

// DEVICE READ
static ssize_t chardev_read(struct file *filep, char __user *buff, size_t len, loff_t *pos)
{	
	pr_info("chardev_read Invoked\n");
	return 0;
}

// DEVICE WRITE
static ssize_t chardev_write(struct file *filep, const char __user *buff, size_t len, loff_t *pos)
{	
	pr_info("chardev_write Invoked\n");
	return 0;	
}

// IOCTL
static long chardev_ioctl(struct file *filep, unsigned int cmd, unsigned long arg)
{
	pr_info("chardev_ioctl Invoked.\n");
	switch(cmd)
	{
		case CMD_1:
			pr_alert("This is CMD_1 command.\n");
			break;
		case CMD_2:
			pr_alert("This is CMD_2 command. Received arg = %d\n",arg);
			break;
		default:
			pr_info("Invalid Command\n");
			break;
	}

	return 0;
}

static struct file_operations chardev_fops = 
{
	.owner = THIS_MODULE,
	.open = chardev_open,
	.release = chardev_release,
	.write = chardev_write,
	.read = chardev_read,
	.unlocked_ioctl = chardev_ioctl,
};

//MODULE INITIALIZATION
static int __init chardev_init(void)
{
	pr_info("chardev_init is Invoked\n");
	alloc_chrdev_region(&dev_num, 0, COUNT, DEV_NAME);
	pr_info("MAJOR DEV_NUM %d\n", MAJOR(dev_num));
	pr_info("MINOR DEV_NUM %d\n", MINOR(dev_num));
	
	cdev_init(&chardev, &chardev_fops);
	cdev_add(&chardev, dev_num,1);

	pr_info("chardev Driver Loaded\n");
	return 0;
}

static void __exit chardev_exit(void)
{
	pr_info("chardev_exit is Invoked\n");
//	device_destroy(chardev,dev_num);
	cdev_del(&chardev);
	unregister_chrdev_region(dev_num,1);
	pr_info("chardev driver unloaded\n");
}

module_init(chardev_init);
module_exit(chardev_exit);

