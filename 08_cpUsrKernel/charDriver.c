
// Character Driver application without echo and cat command and Accessing from user application and storing into kernel space


#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ABHINAV");

#define mem_size 1024 		//Memory size

dev_t dev = 0;

//static struct class *dev class
static struct cdev char_cdev;
uint8_t *kernel_buffer;

/*Function Prototypes*/

static int __init char_driver_init(void);
static void __exit char_driver_exit(void);

static int char_open(struct inode *inode, struct file *file);
static int char_release(struct inode *inode, struct file *file);
static ssize_t char_read(struct file *filep, char __user *buf, size_t len, loff_t * off);
static ssize_t char_write(struct file *filep, const char *buf, size_t len, loff_t * off);

/*File operations structure*/

static struct file_operations fops = 
{
	.owner = THIS_MODULE,
	.read = char_read,
	.write = char_write,
	.open = char_open,
       .release = char_release,                             
};

/* This function will be called when we open the device file */

static int char_open(struct inode *inode, struct file *filep)
{
	pr_info("Device file opened.\n");
	return 0;
}

/* This function will be called when we close the device file */

static int char_release(struct inode *inode, struct file *filep)
{
	pr_info("Device file closed.\n");
	return 0;
}

/* This function will be called when we read the device file */

static ssize_t char_read(struct file *filep, char __user *buf, size_t len, loff_t *off)
{
	//copy the data from the kernel space to user space
	if(copy_to_user(buf, kernel_buffer, mem_size))
	{
		pr_err("Data Read :Err\n");
	}
	
	pr_info("Data Read :Done\n");
	return mem_size;
}

/* This function will be called when we write the Device file...*/

static ssize_t char_write(struct file *filep, const char __user *buf, size_t len, loff_t * off)
{
	//copy the data to kernel space from the user space
        if(copy_from_user(kernel_buffer, buf, len))
	{
		pr_err("Data Write \n");
	}
	
	pr_info("Data Write: Done\n");
	return len;
}

/*....Module Init function */

static int __init char_driver_init(void)
{
	/* Allocating Major Number */
	if((alloc_chrdev_region(&dev, 0, 1, "copyToUSR"))<0)
	{
		pr_info("Cannot allocate major number\n");
		return -1;
	}
	
	pr_info("Major = %d Minor = %d \n", MAJOR(dev), MINOR(dev));
        
	/* creating cdev structure */
	cdev_init(&char_cdev, &fops);

	/*Adding character device to the system */

	if((cdev_add(&char_cdev, dev, 1))<0)
	{
		pr_info("Cannot add the device to the system \n");
	}

	/* Creating physical memory*/
	if((kernel_buffer = kmalloc(mem_size , GFP_KERNEL)) == 0)
	{
		pr_info("Cannot allocate memory in kernel \n");
	}

	strcpy(kernel_buffer, "Hello World");
	pr_info("Device Driver Insert ...done\n");
	return 0;
}

/*.....Module exit functions....*/

static void __exit char_driver_exit(void)
{
	kfree(kernel_buffer);
	cdev_del(&char_cdev);
	unregister_chrdev_region(dev, 1);
	pr_info("Device Driver Removed\n");
}

module_init(char_driver_init);
module_exit(char_driver_exit);
