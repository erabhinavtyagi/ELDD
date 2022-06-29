#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/timer.h>
#include <linux/jiffies.h>

//************* Timer variable ********************

#define TIMEOUT 1000         // 1 second = 1000 ms

static struct timer_list my_timer;
static unsigned int count = 0;

dev_t dev = 0;
static struct class *dev_class;
static struct cdev my_cdev;

static int __init my_driver_init(void);
static void __exit my_driver_exit(void);

/************************** Driver Function Declaration **********************************/

static int my_open(struct inode *inode, struct file *file);
static int my_release(struct inode *inode, struct file *file);
static ssize_t my_read(struct file *file, char __user *ubuff, size_t count, loff_t *off);
static ssize_t my_write(struct file *file, const char __user *ubuff, size_t count, loff_t *off);

struct file_operations fops = 
{
    .owner      = THIS_MODULE,
    .open       = my_open,
    .read       = my_read,
    .write      = my_write,
    .release    = my_release,
};
void timer_callback (struct timer_list *data)
{
    // Do your Timer stuff here
    pr_info("Timer callback function called [%d] \n", count++);
  
    // Re-enable timer. Because this function will be called only first time. If we re-enable 
    // this will work like periodic timer. 
   
   mod_timer(&my_timer, jiffies + msecs_to_jiffies(TIMEOUT));
}

static int my_open(struct inode *inode, struct file *file)
{
    pr_info("OPENING DEVICE FILE...\n");
    return 0;
}

static int my_release(struct inode *inode, struct file *file)
{
    pr_info("CLOSED DEVICE FILE...\n");
    return 0;
}

static ssize_t my_read(struct file *file, char __user *ubuff, size_t count, loff_t *off)
{
    pr_info("IN READ FUNCTION...\n");
    return 0;
}

static ssize_t my_write(struct file *file, const char __user *ubuff, size_t count, loff_t *off)
{
    pr_info("IN WRITE FUNCTION...\n");
    return count;
}

static int my_driver_init(void)
{
// Allocating Major Number
    if(alloc_chrdev_region(&dev, 0, 1, "my_dev_timer")< 0)
    {  
        pr_err("ERROR IN ALLOCATING MAJOR NUMBER\n");
        return -1;
    }
    pr_info("Allocated : MAJOR NUMBER = %d  MINOR NUMBER = %d \n", MAJOR(dev), MINOR(dev));
   
// Creating Device to the system(creating cdev structure)
   cdev_init(&my_cdev,&fops);

// Adding Character Device
   if(cdev_add(&my_cdev,dev,1)<0)
   {
    pr_err("CANNOT ADD DEVICE TO THE SYSTEM\n");
    goto r_class;
   }
   

// Creating Struct Class
   if(dev_class = class_create(THIS_MODULE,"my_class") == NULL)
   {
    pr_err("CANNOT CREATE THE CLASS STRUCTURE\n");
    goto r_class;
   }

//Creating Devices
   if(device_create(dev_class,NULL,dev,NULL,"my_device") == NULL)
   {
    pr_err("CANNOT CREATE THE DEVICE \n");
    goto r_device;
   }
/* Setup your timer to call my timer callback */
    timer_setup(&my_timer,timer_callback,0);
    // if you face some issues and using older kernel version, then you can try setup_timer API
    // (change callback functions argument to unsigned long instead of struct timer list)
    mod_timer(&my_timer, jiffies + msecs_to_jiffies(TIMEOUT));
    pr_info("Device driver insert...done..\n");
    return 0;

    r_device:
        class_destroy(dev_class);
    r_class:
        unregister_chrdev_region(dev,1);
        return -1;
}

static void my_driver_exit(void)
{
// remove kernel timer when unloading module 
del_timer(&my_timer);
device_destroy(dev_class,dev);
class_destroy(dev_class);
cdev_del(&my_cdev);
unregister_chrdev_region(dev,1);
pr_info("Device driver removed.../n");
}

module_init(my_driver_init);
module_exit(my_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Abhinav Tyagi");
MODULE_DESCRIPTION("TIMER DEMO PROGRAM");