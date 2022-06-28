#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h>			// kmalloc()
#include <linux/uaccess.h>		// Used for copy_to / from_user() functions
#include <linux/kthread.h>
#include <linux/wait.h>			// Required for the wait queue

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Abhinav");
MODULE_DESCRIPTION("Wait Queue Static Method.");				

uint32_t read_count = 0;
static struct task_struct *wait_thread;

DECLARE_WAIT_QUEUE_HEAD(wait_queue_stat);

dev_t dev = 0;
static struct class *dev_class;
static struct cdev stat_cdev;
int wait_queue_flag = 0;

// Function Prototypes

static int 	__init stat_driver_init(void);
static void 	__exit stat_driver_exit(void);

//-------------- DRIVER FUNCTIONS-------------------------------------

static int 	stat_open(struct inode *inode, struct file *file);
static int 	stat_release(struct inode *inode, struct file *file);
static ssize_t 	stat_read(struct file *filp, char __user *buf, size_t len, loff_t * off);
static ssize_t 	stat_write(struct file *filp, const char *buf, size_t len, loff_t * off);

// FILE OPERATION STRUCTURE

static struct file_operations fops =
{
	.owner		= THIS_MODULE,
	.read		= stat_read,
	.write		= stat_write,
	.open		= stat_open,
	.release 	= stat_release,
};

// THREAD FUNCTION

static int wait_function(void *unused)
{
	while(1)
	{
	pr_info("Waiting for Event.\n");
	wait_event_interruptible(wait_queue_stat, wait_queue_flag != 0);
	
	if(wait_queue_flag == 2)
	{
		pr_info("Event came from Exit Function.\n");
		return 0;
	}
	pr_info("Event came from Read Function: %d\n", ++read_count);
	wait_queue_flag = 0;
	}
	do_exit(0);
	return 0;
}

// OPEN THE DEVICE FILE

static int stat_open(struct inode *inode, struct file *file)
{
	pr_info("Device File Opened...\n");
	return 0;
}

// CLOSE THE DEVICE FILE

static int stat_release(struct inode *inode, struct file *file)
{
	pr_info("Device File Closed...\n");
	return 0;
}

// READ THE DEVICE FILE

static ssize_t stat_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
	pr_info("Read Function\n");
	wait_queue_flag = 1;
	wake_up_interruptible(&wait_queue_stat);
	return 0;
}

// WRITE THE DEVICE FILE

static ssize_t stat_write(struct file *filp, const char __user *buf, size_t len, loff_t *off)
{
	pr_info("Write Function\n");
	return len;
}

// ------------------- MODULE INITIALIZATION -------------------------

static int __init stat_driver_init(void)
{
	// Allocating Major Number
	if ((alloc_chrdev_region(&dev, 0, 1, "stat_Dev")) < 0 )
	{
	pr_info("Can not allocate Major Number.\n");
	return -1;
	}
	
	pr_info("Major Number: %d  Minor Number: %d \n", MAJOR(dev),MINOR(dev));

	// CREATING CDEV STRUCTURE
	cdev_init(&stat_cdev, &fops);
	stat_cdev.owner = THIS_MODULE;
	stat_cdev.ops = &fops;

	// ADDING CHARACTER DEVICE TO THE SYSTEM
	if((cdev_add(&stat_cdev, dev, 1))<0)
	{
	pr_info("Can not add the device to the system.\n");
	goto r_class;
	}

	// CREATING STRUCT CLASS
	if((dev_class = class_create(THIS_MODULE,"waitQ_dev")) == NULL)
	{
	pr_info("Can not create the struct class\n");
	goto r_class;
	}	

	// CREATING DEVICE
	if ((device_create(dev_class, NULL, dev, NULL,"waitQ_dev")) == NULL)
	{
	pr_info("Can not create the Device 1\n");
	goto r_device;
	}

	// CREATING THE KERNEL THREAD WITH NAME 'mythread'
	wait_thread = kthread_create(wait_function, NULL, "WaitThread");
	if(wait_thread)
	{
	pr_info("Thread Created Successfully\n");
	wake_up_process(wait_thread);
	}
	else
		pr_info("Thread Creation Failed\n");
	
	pr_info("Device Driver Insert.. Done!!\n");
	return 0;
	

r_device:
	class_destroy(dev_class);

r_class:
	unregister_chrdev_region(dev,1);
	return -1;
}

//----------- Module Exit Function --------------------------------------

static void __exit stat_driver_exit(void)
{
	wait_queue_flag = 2;
	wake_up_interruptible(&wait_queue_stat);
	device_destroy(dev_class, dev);
	class_destroy(dev_class);
	cdev_del(&stat_cdev);
	unregister_chrdev_region(dev, 1);
	pr_info("Device Driver Remove...Done\n");
}

module_init(stat_driver_init);
module_exit(stat_driver_exit);
