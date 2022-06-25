/****************************************************************************//**
*  \file       dynam.c
*
*  \details    Simple linux driver (Waitqueue Dynamic method)
*
*  \author     ABHINAV
*
*  \steps for compiling and loading
* 
* 1.make
* 2.sudo -s
* 3.dmesg -C
* 4.dmesg
* 5.echo "0" > /dev/realdriver //for write
* 6.dmesg
* 7.cat /dev/realdriver    //for read
* 8.dmesg
* 9.rmmod dynam
* 10.dmesg
* 11.exit 
*
*******************************************************************************/
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h>                 //kmalloc()
#include <linux/uaccess.h>              //copy_to/from_user()
#include <linux/kthread.h>
#include <linux/wait.h>                 // Required for the wait queues
 
 
uint32_t read_count = 0;
uint32_t write_count = 0;
static struct task_struct *wait_thread;
 
dev_t dev = 0;
static struct class *dev_class;
static struct cdev aj_cdev;
wait_queue_head_t wait_queue_aj;
int wait_queue_flag = 0;
 
/*
** Function Prototypes
*/
static int      __init aj_driver_init(void);
static void     __exit aj_driver_exit(void);
 
/*************** Driver functions **********************/
static int      aj_open(struct inode *inode, struct file *file);
static int      aj_release(struct inode *inode, struct file *file);
static ssize_t  aj_read(struct file *filp, char __user *buf, size_t len,loff_t * off);
static ssize_t  aj_write(struct file *filp, const char *buf, size_t len, loff_t * off);

/*
** File operation sturcture
*/
static struct file_operations fops =
{
        .owner          = THIS_MODULE,
        .read           = aj_read,
        .write          = aj_write,
        .open           = aj_open,
        .release        = aj_release,
};
 
/*
** Thread function
*/
static int wait_function(void *unused)
{
        
        while(1) {
                pr_info("Waiting For Event...\n");
                wait_event_interruptible(wait_queue_aj, wait_queue_flag != 0 );
                if(wait_queue_flag == 2) {
                        pr_info("Event Came From Exit Function\n");
                        return 0;
                }
		if(wait_queue_flag == 1)
		{
		pr_info("Event Came From Read Function - %d\n", ++read_count);
		}
		if(wait_queue_flag == 3)
		{
			pr_info("Event came from write function\n",++write_count);
			wait_queue_flag = 0;
		}
		wait_queue_flag = 0;
        }
        return 0;
}
 
/*
** This function will be called when we open the Device file
*/ 
static int aj_open(struct inode *inode, struct file *file)
{
        pr_info("Device File Opened...!!!\n");
        return 0;
}

/*
** This function will be called when we close the Device file
*/
static int aj_release(struct inode *inode, struct file *file)
{
        pr_info("Device File Closed...!!!\n");
        return 0;
}

/*
** This function will be called when we read the Device file
*/
static ssize_t aj_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
        pr_info("Read Function\n");
        wait_queue_flag = 1;
        wake_up_interruptible(&wait_queue_aj);
        return 0;
}

/*
** This function will be called when we write the Device file
*/
static ssize_t aj_write(struct file *filp, const char __user *buf, size_t len, loff_t *off)
{
        pr_info("Write function\n");
        wait_queue_flag = 3;
	wake_up_interruptible(&wait_queue_aj);
	return len;
}

/*
** Module Init function
*/
static int __init aj_driver_init(void)
{
        /*Allocating Major number*/
        if((alloc_chrdev_region(&dev, 0, 1, "aj_Dev")) <0){
                pr_info("Cannot allocate major number\n");
                return -1;
        }
        pr_info("Major = %d Minor = %d \n",MAJOR(dev), MINOR(dev));
 
        /*Creating cdev structure*/
        cdev_init(&aj_cdev,&fops);
 
        /*Adding character device to the system*/
        if((cdev_add(&aj_cdev,dev,1)) < 0){
            pr_info("Cannot add the device to the system\n");
            goto r_class;
        }
 
        /*Creating struct class*/
        if((dev_class = class_create(THIS_MODULE,"aj_class")) == NULL){
            pr_info("Cannot create the struct class\n");
            goto r_class;
        }
 
        /*Creating device*/
        if((device_create(dev_class,NULL,dev,NULL,"aj_device")) == NULL){
            pr_info("Cannot create the Device 1\n");
            goto r_device;
        }
        
        //Initialize wait queue
        init_waitqueue_head(&wait_queue_aj);
 
        //Create the kernel thread with name 'mythread'
        wait_thread = kthread_create(wait_function, NULL, "WaitThread");
        if (wait_thread) {
                pr_info("Thread Created successfully\n");
                wake_up_process(wait_thread);
        } else
                pr_info("Thread creation failed\n");
 
        pr_info("Device Driver Insert...Done!!!\n");
        return 0;
 
r_device:
        class_destroy(dev_class);
r_class:
        unregister_chrdev_region(dev,1);
        return -1;
}

/*
** Module exit function
*/
static void __exit aj_driver_exit(void)
{
        wait_queue_flag = 2;
        wake_up_interruptible(&wait_queue_aj);
        device_destroy(dev_class,dev);
        class_destroy(dev_class);
        cdev_del(&aj_cdev);
        unregister_chrdev_region(dev, 1);
        pr_info("Device Driver Remove...Done!!!\n");
}
 
module_init(aj_driver_init);
module_exit(aj_driver_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("ABHINAV");
MODULE_DESCRIPTION("Simple linux driver (Waitqueue Dynamic method)");


